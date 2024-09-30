#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/stack.h"
#include "../headers/myRecalloc.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
#ifndef DEBUG_SWITCH_OFF

    #define STACK_SOFT_ASSERT(stack)         \
        stackError_t STACK_ERROR = OK;       \
        STACK_ERROR = StackVerify(stack);    \
        if (STACK_ERROR != OK)               \
            return STACK_ERROR;

#else

    #define STACK_SOFT_ASSERT(stack) 

#endif // DEBUG_SWITCH_OFF


//----------------------------------------------------------------------------------------


/**
 * 
 */
#ifndef DEBUG_SWITCH_OFF
struct StackInfo
{
    const char* name;   /**<  */
    Place       place;  /**<  */
};
#endif // DEBUG_SWITCH_OFF


/**
 * 
 */
struct Stack 
{
    void*      dataBuffer;     /**< Pointer to buffer with stack's data.        */
    size_t     bufferCapacity; /**< Max count of elems which stack can contain. */
    size_t     elemSize;       /**< Size of each elem in bytes.                 */
    size_t     elemCount;      /**< Count of elems in stack.                    */

    ON_DEBUG(StackInfo* stackInfo;)     /**< Info about first stack definition.*/
};


//----------------------------------------------------------------------------------------


/**
 * Minimum stack's bufferSize.
 */
static const size_t MIN_STACK_SIZE = 16;


//----------------------------------------------------------------------------------------


/**
 * 
 */
static bool StackIsInit(Stack* stack);


/**
 * 
 */
ON_DEBUG(static void StackInfoPrint(StackInfo* stackInfo);)


/**
 * 
 */
ON_DEBUG(static void StackPrintContent(Stack* stack);)


/**
 *
 */
static bool StackIsExpandNeed(Stack* stack);


/**
 * 
 */
static bool StackIsCompressNeed(Stack* stack);


/**
 * 
 */
static stackError_t StackExpand(Stack* stack);


/**
 * 
 */
static stackError_t StackCompress(Stack* stack);


/**
 * 
 */
ON_DEBUG(static void StackPrintFields(Stack* stack);)


/**
 * 
 */
ON_DEBUG(static stackError_t StackVerify(const Stack* stack);)


/**
 * 
 */
ON_DEBUG(static stackError_t StackInfoVerify(const StackInfo* stackInfo);)


//----------------------------------------------------------------------------------------


stackError_t StackCreate(Stack**      stack, ON_DEBUG(StackInfo* stackInfo,) 
                         const size_t elemSize)
{
    #ifndef DEBUG_SWITCH_OFF
    if (stack == NULL)
        return STACK_NULL_PTR;

    if (StackIsInit(*stack))
        return IS_INIT;
    #endif // DEBUG_SWITCH_OFF

    *stack = (Stack*) calloc(1, sizeof(Stack));
    if (*stack == NULL) 
        return ALLOCATE_ERROR;

    (*stack)->bufferCapacity = MIN_STACK_SIZE;
    (*stack)->elemCount      = 0;
    (*stack)->elemSize       = elemSize;
    (*stack)->dataBuffer     = calloc(MIN_STACK_SIZE, elemSize);

    ON_DEBUG((*stack)->stackInfo = stackInfo;)

    if ((*stack)->dataBuffer == NULL)
    {
        StackDelete(stack);
        return ALLOCATE_ERROR;
    }

    return OK;
}


#ifndef DEBUG_SWITCH_OFF
StackInfo* StackInfoGet(const char* stackName, const Place place)
{
    assert(stackName);

    StackInfo* stackInfo = (StackInfo*) calloc(1, sizeof(StackInfo));

    stackInfo->name  = stackName;
    stackInfo->place = place;  

    return stackInfo;
}
#endif // DEBUG_SWITCH_OFF


stackError_t StackDelete(Stack** stack)
{
    #ifndef DEBUG_SWITCH_OFF
    if (stack == NULL || *stack == NULL)
        return STACK_NULL_PTR;
    #endif // DEBUG_SWITCH_OFF

    free((*stack)->dataBuffer);
    ON_DEBUG(free((*stack)->stackInfo);)

    (*stack)->dataBuffer = NULL;
    ON_DEBUG((*stack)->stackInfo = NULL;)

    (*stack)->bufferCapacity = 0;
    (*stack)->elemCount      = 0;
    (*stack)->elemSize       = 0;

    free(*stack);
    *stack = NULL;

    return OK;
}


stackError_t StackPop(Stack* stack, void* elemBufferPtr)
{
    STACK_SOFT_ASSERT(stack);

    if (stack->elemCount == 0)
        return UNDERFLOW;
    
    if (StackIsCompressNeed(stack))         
    {                           
        stackError_t stackError = OK;            
        stackError = StackCompress(stack);  
        if (stackError != OK)               
            return stackError;              
    }

    void* stackElemPtr = (char*) stack->dataBuffer + (stack->elemCount - 1) * 
                                                                        stack->elemSize;

    if (memmove(elemBufferPtr, stackElemPtr, stack->elemSize) == NULL)
        return ALLOCATE_ERROR;

    if (memset(stackElemPtr, 0, stack->elemSize) == NULL)
        return ALLOCATE_ERROR;

    stack->elemCount -= 1;

    return OK;
}


stackError_t StackPush(Stack* stack, void* elemPtr)
{
    STACK_SOFT_ASSERT(stack);

    if (StackIsExpandNeed(stack))
    {
        stackError_t stackError = OK;
        stackError = StackExpand(stack);

        if (stackError != OK)
            return stackError;
    }

    void* stackElemPtr = (char*) stack->dataBuffer + stack->elemCount * stack->elemSize;

    if (memmove(stackElemPtr, elemPtr, stack->elemSize) == NULL)
        return ALLOCATE_ERROR;

    stack->elemCount += 1;

    return OK;
}


#ifndef DEBUG_SWITCH_OFF
void StackDump(Stack* stack, Place place) 
{
    if (stack == NULL)
    {
        LogPrint(WARNING, place, "Stack* stack == NULL");
        return;
    }

    LogPrint(INFO, place, "Stack's dumping...\n");

    StackPrintFields(stack);
    StackPrintContent(stack);
}
#endif // DEBUG_SWITCH_OFF


const char* StackGetErrorCode(const stackError_t stackError)
{
    switch (stackError)
    {
    case OK: 
        return "OK";

    case IS_INIT:
        return "IS_INIT";

    case ALLOCATE_ERROR:
        return "ALLOCATE_ERROR";

    case UNDERFLOW:
        return "UNDERFLOW";
        
    case STACK_NULL_PTR:
        return "STACK_NULL_PTR";

    case BUFFER_NULL_PTR:
        return "BUFFER_NULL_PTR";

    case CAPACITY_OVERFLOW:
        return "CAPACITY_OVERFLOW";

    case ELEM_SIZE_OVERFLOW:
        return "ELEM_SIZE_OVERFLOW";

    case ELEM_COUNT_OVERFLOW:
        return "ELEM_COUNT_OVERFLOW";


    #ifndef DEBUG_SWITCH_OFF
    case STACK_INFO_NULL_PTR:
        return "STACK_INFO_NULL_PTR";

    case STACK_INFO_NULL_NAME:
        return "STACK_INFO_NULL_NAME";

    case STACK_INFO_WRONG_PLACE:
        return "STACK_INFO_WRONG_PLACE";
    #endif // DEBUG_SWITCH_OFF


    default:
        return "WRONG_STACK_ERROR_CODE";
    }
}


//----------------------------------------------------------------------------------------


static bool StackIsInit(Stack* stack)
{
    if (stack == NULL)
        return false;

    if (stack->dataBuffer     == NULL &&
        stack->bufferCapacity == 0    &&
        stack->elemCount      == 0    &&
        stack->elemSize       == 0)
    {
        return false;
    }

    return true;
}


static bool StackIsExpandNeed(Stack* stack)
{
    if (stack->elemCount == stack->bufferCapacity)
        return true;

    return false;
}


static bool StackIsCompressNeed(Stack* stack)
{
    if (stack->bufferCapacity / 4 >= stack->elemCount &&
        stack->bufferCapacity     >= 2 * MIN_STACK_SIZE)
    {
        return true;
    }

    return false;
}


static stackError_t StackExpand(Stack* stack)
{                       
    void* newDataBuffer = MyRecalloc(stack->dataBuffer, stack->bufferCapacity, 
                                     stack->bufferCapacity * 2, stack->elemSize);
    if (newDataBuffer == NULL)
        return ALLOCATE_ERROR;

    if (stack->dataBuffer != newDataBuffer)
        stack->dataBuffer = newDataBuffer;

    stack->bufferCapacity *= 2;

    return OK;
}


static stackError_t StackCompress(Stack* stack)
{   
    void* newDataBuffer = MyRecalloc(stack->dataBuffer, stack->bufferCapacity,
                                     stack->bufferCapacity / 2, stack->elemSize);

    if (newDataBuffer == NULL)
        return ALLOCATE_ERROR;

    if (stack->dataBuffer != newDataBuffer)
        stack->dataBuffer = newDataBuffer;

    stack->bufferCapacity /= 2;

    return OK;
}


#ifndef DEBUG_SWITCH_OFF
static void StackInfoPrint(StackInfo* stackInfo)
{
    if (stackInfo == NULL)
    {
        LOG_PRINT(ERROR, "StackInfo* stackInfo == NULL.\n");
        return;
    }

    LOG_DUMMY_PRINT("\tStack %s was created in %s: %s(): line %d\n\n",
                    stackInfo->name,           stackInfo->place.file, 
                    stackInfo->place.function, stackInfo->place.line);
}
#endif // DEBUG_SWITCH_OFF


#ifndef DEBUG_SWITCH_OFF
static void StackPrintContent(Stack* stack)
{
    stackError_t stackError = OK;
    stackError = StackVerify(stack);
    if (stackError != OK)
    {
        LOG_PRINT(ERROR, "Stack wasn't verified, error code = %s.\n", 
                                                        StackGetErrorCode(stackError));
        return;
    }

    const size_t elemCount  = stack->elemCount;
    const size_t elemSize   = stack->elemSize;
    char*        dataBuffer = (char*) (stack->dataBuffer);
    char*        format     = GET_ARRAY_PRINTING_FORMAT(stack->elemCount);

    for (size_t elemNum = 0; elemNum < elemCount; elemNum++)
    {
        LOG_DUMMY_PRINT(format, elemNum);
        LOG_DUMMY_PRINT("0x");
        LOG_PRINT_ELEM(dataBuffer, elemSize);
        LOG_DUMMY_PRINT("\n");

        dataBuffer += elemSize;
    }
    LOG_DUMMY_PRINT("\n");

    free(format);
}
#endif // DEBUG_SWITCH_OFF


#ifndef DEBUG_SWITCH_OFF
static void StackPrintFields(Stack* stack)
{
    StackInfoPrint(stack->stackInfo);

    LOG_DUMMY_PRINT("\tdataBuffer = %p \n"
                    "\tbufferSize = %zu \n"
                    "\telemSize = %zu \n"
                    "\telemCount = %zu \n\n",
                    stack->dataBuffer, 
                    stack->bufferCapacity,
                    stack->elemSize,
                    stack->elemCount);
}
#endif // DEBUG_SWITCH_OFF


#ifndef DEBUG_SWITCH_OFF
static stackError_t StackVerify(const Stack* stack)
{
    const size_t maxSizeValue = __SIZE_MAX__ / 2 - 1;

    if (stack == NULL)
        return STACK_NULL_PTR;
    
    if (stack->dataBuffer == NULL)
        return BUFFER_NULL_PTR;

    if (stack->bufferCapacity >= maxSizeValue)
        return CAPACITY_OVERFLOW;

    if (stack->elemCount >= maxSizeValue)
        return ELEM_COUNT_OVERFLOW;
    
    if (stack->elemSize >= maxSizeValue)
        return ELEM_SIZE_OVERFLOW;

    #ifndef DEBUG_SWITCH_OFF
    stackError_t stackError = OK;
    stackError = StackInfoVerify(stack->stackInfo);
    if (stackError != OK)
        return stackError;
    #endif // DEBUG_SWITCH_OFF

    return OK;
}
#endif // DEBUG_SWITCH_OFF


#ifndef DEBUG_SWITCH_OFF
static stackError_t StackInfoVerify(const StackInfo* stackInfo)
{
    if (stackInfo == NULL)
        return STACK_INFO_NULL_PTR;
    
    if (stackInfo->name == NULL)
        return STACK_INFO_NULL_NAME;

    if (!IsPlaceCorrect(stackInfo->place))
        return STACK_INFO_WRONG_PLACE;

    return OK;
}
#endif // DEBUG_SWITCH_OFF
