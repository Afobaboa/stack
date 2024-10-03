#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/stack.h"
#include "../headers/myRecalloc.h"

#ifndef CANARY_SWITCH_OFF
#include "../headers/canary.h"
#endif // CANARY_SWITCH_OFF

#ifndef HASH_SWITCH_OFF
#include "../headers/hash.h"
#endif // HASH_SWITCH_OFF


//----------------------------------------------------------------------------------------


/**
 * 
 */
#define STACK_SOFT_ASSERT(stack)         \
    stackError_t STACK_ERROR = OK;       \
    STACK_ERROR = StackVerify(stack);    \
    if (STACK_ERROR != OK)               \
        return STACK_ERROR;


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
    #ifndef CANARY_SWITCH_OFF
    canary_t leftCanary;        /**<  */
    #endif // CANARY_SWITCH_OFF

    void*      dataBuffer;      /**< Pointer to buffer with stack's data.        */
    size_t     bufferCapacity;  /**< Max count of elems which stack can contain. */
    size_t     elemSize;        /**< Size of each elem in bytes.                 */
    size_t     elemCount;       /**< Count of elems in stack.                    */

    #ifndef DEBUG_SWITCH_OFF
    StackInfo* stackInfo;       /**< Info about first stack definition.*/
    #endif // DEBUG_SWITCH_OFF

    #ifndef HASH_SWITCH_OFF
    hashData_t structHash;      /**<  */
    hashData_t dataHash;        /**<  */
    #endif // HASH_SWITCH_OFF

    #ifndef CANARY_SWITCH_OFF
    canary_t rightCanary;       /**<  */
    #endif // CANARY_SWITCH_OFF
};


//----------------------------------------------------------------------------------------


/**
 * Minimum stack's bufferSize.
 */
static const size_t MIN_STACK_CAPACITY = 16;


//----------------------------------------------------------------------------------------


/**
 * 
 */
static bool StackIsInit(Stack* stack);


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
static stackError_t StackVerify(Stack* stack);


//----------------------------------------------------------------------------------------


#ifndef DEBUG_SWITCH_OFF

/**
 * 
 */
static void StackPrintFields(Stack* stack);


/**
 * 
 */
static void StackInfoPrint(StackInfo* stackInfo);


/**
 * 
 */
static void StackPrintContent(Stack* stack);


/**
 * 
 */
static stackError_t StackInfoVerify(const StackInfo* stackInfo);

#endif // DEBUG_SWITCH_OFF


//----------------------------------------------------------------------------------------


#ifndef CANARY_SWITCH_OFF

/**
 * 
 */
static stackError_t StackCanaryCheck(Stack* stack);

#endif // CANARY_SWITCH_OFF


//----------------------------------------------------------------------------------------


#ifndef HASH_SWITCH_OFF

/**
 * 
 */
static hashData_t StackGetStructHash(Stack* stack);


/**
 * 
 */
static hashData_t StackGetDataHash(Stack* stack);


/**
 * 
 */
static void StackHashSet(Stack* stack);


/**
 * 
 */
static void StackHashDelete(Stack* stack);


/**
 * 
 */
static stackError_t StackCheckHash(Stack* stack);


/**
 * 
 */
static void DoHash(hashData_t* hashBuffer, 
                   hashData_t* dataPtr, const size_t dataByteCount);

#endif // HASH_SWITCH_OFF




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

    (*stack)->bufferCapacity = MIN_STACK_CAPACITY;
    (*stack)->elemCount      = 0;
    (*stack)->elemSize       = elemSize;
    (*stack)->dataBuffer     = calloc(MIN_STACK_CAPACITY * elemSize
                                                    CANARY( + 2 * sizeof(canary_t)), 1);

    if ((*stack)->dataBuffer == NULL)
    {
        StackDelete(stack);
        return ALLOCATE_ERROR;
    }

    #ifndef CANARY_SWITCH_OFF
    CanarySet((canary_t*) ((*stack)->dataBuffer));
    CanarySet((canary_t*) ((char*) ((*stack)->dataBuffer) + sizeof(canary_t) + 
                                                    elemSize * MIN_STACK_CAPACITY));
    #endif // CANARY_SWITCH_OFF

    #ifndef DEBUG_SWITCH_OFF
    (*stack)->stackInfo = stackInfo;
    #endif // DEBUG_SWITCH_OFF

    #ifndef CANARY_SWITCH_OFF
    CanarySet(&((*stack)->leftCanary));
    CanarySet(&((*stack)->rightCanary));
    #endif // CANARY_SWITCH_OFF

    #ifndef HASH_SWITCH_OFF
    StackHashSet(*stack);
    #endif // HASH_SWITCH_OFF

    STACK_SOFT_ASSERT(*stack);

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

    #ifndef HASH_SWITCH_OFF

    if (CANARY(StackCanaryCheck(*stack) == OK &&) 
        StackCheckHash(*stack) == OK)

    #else
    
    #ifndef CANARY_SWITCH_OFF
    if (StackCanaryCheck(*stack) == OK)
    #endif // CANARY_SWITCH_OFF

    #endif // HASH_SWITCH_OFF
        free((*stack)->dataBuffer);
    (*stack)->dataBuffer = NULL;

    #ifndef DEBUG_SWITCH_OFF
    free((*stack)->stackInfo);
    (*stack)->stackInfo = NULL;
    #endif // DEBUG_SWITCH_OFF

    (*stack)->bufferCapacity = 0;
    (*stack)->elemCount      = 0;
    (*stack)->elemSize       = 0;

    #ifndef CANARY_SWITCH_OFF
    CanaryDelete(&((*stack)->leftCanary));
    CanaryDelete(&((*stack)->rightCanary));
    #endif // CANARY_SWITCH_OFF
    
    #ifndef HASH_SWITCH_OFF
    StackHashDelete(*stack);
    #endif // HASH_SWITCH_OFF

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

    void* stackElemPtr = (char*) stack->dataBuffer + CANARY(sizeof(canary_t) + )
                                            (stack->elemCount - 1) * stack->elemSize;

    if (memmove(elemBufferPtr, stackElemPtr, stack->elemSize) == NULL)
        return ALLOCATE_ERROR;

    if (memset(stackElemPtr, 0, stack->elemSize) == NULL)
        return ALLOCATE_ERROR;

    stack->elemCount -= 1;

    #ifndef HASH_SWITCH_OFF
    StackHashSet(stack);
    #endif // HASH_SWITCH_OFF

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

    void* stackElemPtr = (char*) stack->dataBuffer + CANARY(sizeof(canary_t) + )
                                                     stack->elemCount * stack->elemSize;

    if (memmove(stackElemPtr, elemPtr, stack->elemSize) == NULL)
        return ALLOCATE_ERROR;

    stack->elemCount += 1;

    #ifndef HASH_SWITCH_OFF
    StackHashSet(stack);
    #endif // HASH_SWITCH_OFF

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

    LogPrint(INFO, place, "Stack dumping starts...\n");

    StackPrintFields(stack);
    StackPrintContent(stack);

    LogDummyPrint(place, "\tStack dumping end.\n");
}
#endif // DEBUG_SWITCH_OFF


const char* StackGetErrorName(const stackError_t stackError)
{
    switch (stackError)
    {
    case OK: 
        return GET_NAME(OK);

    case IS_INIT:
        return GET_NAME(IS_INIT);

    case ALLOCATE_ERROR:
        return GET_NAME(ALLOCATE_ERROR);

    case UNDERFLOW:
        return GET_NAME(UNDERFLOW);
        
    case STACK_NULL_PTR:
        return GET_NAME(STACK_NULL_PTR);

    case BUFFER_NULL_PTR:
        return GET_NAME(BUFFER_NULL_PTR);

    case CAPACITY_OVERFLOW:
        return GET_NAME(CAPACITY_OVERFLOW);

    case ELEM_SIZE_OVERFLOW:
        return GET_NAME(ELEM_SIZE_OVERFLOW);

    case ELEM_COUNT_OVERFLOW:
        return GET_NAME(ELEM_COUNT_OVERFLOW);


    #ifndef DEBUG_SWITCH_OFF
    case STACK_INFO_NULL_PTR:
        return GET_NAME(STACK_INFO_NULL_PTR);

    case STACK_INFO_NULL_NAME:
        return GET_NAME(STACK_INFO_NULL_NAME);

    case STACK_INFO_WRONG_PLACE:
        return GET_NAME(STACK_INFO_WRONG_PLACE);
    #endif // DEBUG_SWITCH_OFF


    #ifndef CANARY_SWITCH_OFF
    case STACK_CANARY_RIGHT_SPOILED:
        return GET_NAME(STACK_CANARY_RIGHT_SPOILED);
    
    case STACK_CANARY_LEFT_SPOILED:
        return GET_NAME(STACK_CANARY_LEFT_SPOILED);

    case DATA_CANARY_RIGHT_SPOILED:
        return GET_NAME(DATA_CANARY_RIGHT_SPOILED);

    case DATA_CANARY_LEFT_SPOILED:
        return GET_NAME(DATA_CANARY_LEFT_SPOILED);
    #endif // CANARY_SWITCH_OFF


    #ifndef HASH_SWITCH_OFF
    case STACK_STRUCT_HASH_WRONG:
        return GET_NAME(STACK_STRUCT_HASH_WRONG);
    
    case STACK_DATA_HASH_WRONG:
        return GET_NAME(STACK_DATA_HASH_WRONG);
    #endif // HASH_SWITCH_OFF


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
        stack->elemSize       == 0    

        #ifndef DEBUG_SWITCH_OFF
        && stack->stackInfo == NULL
        #endif // DEBUG_SWITCH_OFF

        #ifndef CANARY_SWITCH_OFF
        && stack->leftCanary  == 0
        && stack->rightCanary == 0
        #endif // CANARY_SWITCH_OFF
        )
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
        stack->bufferCapacity     >= 2 * MIN_STACK_CAPACITY)
    {
        return true;
    }

    return false;
}


static stackError_t StackExpand(Stack* stack)
{                       
    void* newDataBuffer = MyRecalloc(stack->dataBuffer, 
                                     stack->bufferCapacity * stack->elemSize 
                                                        CANARY( + 1 * sizeof(canary_t)), 
                                     stack->bufferCapacity * 2 * stack->elemSize
                                                        CANARY( + 2 * sizeof(canary_t)), 
                                     1);
    if (newDataBuffer == NULL)
        return ALLOCATE_ERROR;

    if (stack->dataBuffer != newDataBuffer)
        stack->dataBuffer = newDataBuffer;

    stack->bufferCapacity *= 2;

    #ifndef CANARY_SWITCH_OFF
    CanarySet((canary_t*) ((char*) stack->dataBuffer + 
                            stack->bufferCapacity * stack->elemSize + sizeof(canary_t)));
    #endif // CANARY_SWITCH_OFF

    return OK;
}


static stackError_t StackCompress(Stack* stack)
{   
    void* newDataBuffer = MyRecalloc(stack->dataBuffer, 
                                     stack->bufferCapacity * stack->elemSize
                                                       CANARY( + 1 * sizeof(canary_t)),
                                     stack->bufferCapacity / 2 * stack->elemSize
                                                       CANARY( + 2 * sizeof(canary_t)), 
                                     1);

    if (newDataBuffer == NULL)
        return ALLOCATE_ERROR;

    if (stack->dataBuffer != newDataBuffer)
        stack->dataBuffer = newDataBuffer;

    stack->bufferCapacity /= 2;

    #ifndef CANARY_SWITCH_OFF
    CanarySet((canary_t*) ((char*) stack->dataBuffer + 
                            stack->bufferCapacity * stack->elemSize + sizeof(canary_t)));
    #endif // CANARY_SWITCH_OFF

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
                                                        StackGetErrorName(stackError));
        return;
    }

    const size_t elemCount  = stack->elemCount;
    const size_t elemSize   = stack->elemSize;
    char*        dataBuffer = (char*) (stack->dataBuffer);
    char*        format     = GET_ARRAY_PRINTING_FORMAT(stack->elemCount);

    #ifndef CANARY_SWITCH_OFF
    LOG_DUMMY_PRINT("dataBuffer: left canary = 0x");
    LOG_PRINT_ELEM(dataBuffer, sizeof(canary_t));
    LOG_DUMMY_PRINT("\n");
    dataBuffer += sizeof(canary_t);
    #endif // CANARY_SWITCH_OFF

    for (size_t elemNum = 0; elemNum < elemCount; elemNum++)
    {
        LOG_DUMMY_PRINT(format, elemNum);
        LOG_DUMMY_PRINT("0x");
        LOG_PRINT_ELEM(dataBuffer, elemSize);
        LOG_DUMMY_PRINT("\n");

        dataBuffer += elemSize;
    }
    LOG_DUMMY_PRINT("\n");

    #ifndef CANARY_SWITCH_OFF
    LOG_DUMMY_PRINT("dataBuffer: right canary = 0x");
    LOG_PRINT_ELEM((char*) stack->dataBuffer + sizeof(canary_t) + 
                           stack->bufferCapacity * stack->elemSize,
                   sizeof(canary_t));
    LOG_DUMMY_PRINT("\n\n");
    #endif // CANARY_SWITCH_OFF

    free(format);
}
#endif // DEBUG_SWITCH_OFF


#ifndef DEBUG_SWITCH_OFF
static void StackPrintFields(Stack* stack)
{
    StackInfoPrint(stack->stackInfo);

    #ifndef CANARY_SWITCH_OFF
    LOG_DUMMY_PRINT(("leftCanary = 0x"));
    LOG_PRINT_ELEM(&(stack->leftCanary), sizeof(canary_t));
    LOG_DUMMY_PRINT("\n");
    #endif // CANARY_SWITCH_OFF

    LOG_DUMMY_PRINT("\tdataBuffer = %p \n"
                    "\tbufferSize = %zu \n"
                    "\telemSize = %zu \n"
                    "\telemCount = %zu \n",
                    stack->dataBuffer, 
                    stack->bufferCapacity,
                    stack->elemSize,
                    stack->elemCount);

    #ifndef HASH_SWITCH_OFF
    LOG_DUMMY_PRINT("\tstructHash = %zu\n"
                    "\tdataHash   = %zu\n",
                    stack->structHash,
                    stack->dataHash);
    #endif // HASH_SWITCH_OFF

    #ifndef CANARY_SWITCH_OFF
    LOG_DUMMY_PRINT(("rightCanary = 0x"));
    LOG_PRINT_ELEM(&(stack->rightCanary), sizeof(canary_t));
    LOG_DUMMY_PRINT("\n");
    #endif // CANARY_SWITCH_OFF

    LOG_DUMMY_PRINT("\n");
}
#endif // DEBUG_SWITCH_OFF


static stackError_t StackVerify(Stack* stack)
{
    stackError_t stackError = OK;

    if (stack == NULL)
        return STACK_NULL_PTR;

    #ifndef HASH_SWITCH_OFF
    stackError = StackCheckHash(stack);
    if (stackError != OK)
        return stackError;
    #endif // HASH_SWITCH_OFF

    #ifndef CANARY_SWITCH_OFF
    stackError = StackCanaryCheck(stack);
    if (stackError != OK)
        return stackError;
    #endif // CANARY_SWITCH_OFF

    #ifndef DEBUG_SWITCH_OFF
    const size_t maxSizeValue = __SIZE_MAX__ / 2 - 1;
    
    if (stack->dataBuffer == NULL)
        return BUFFER_NULL_PTR;

    if (stack->bufferCapacity >= maxSizeValue)
        return CAPACITY_OVERFLOW;

    if (stack->elemCount >= maxSizeValue)
        return ELEM_COUNT_OVERFLOW;
    
    if (stack->elemSize >= maxSizeValue)
        return ELEM_SIZE_OVERFLOW;

    stackError = StackInfoVerify(stack->stackInfo);
    if (stackError != OK)
        return stackError;
    #endif // DEBUG_SWITCH_OFF


    return OK;
}


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


//----------------------------------------------------------------------------------------


#ifndef CANARY_SWITCH_OFF
static stackError_t StackCanaryCheck(Stack* stack)
{
    if (!CanaryCheck(&(stack->leftCanary)))
        return STACK_CANARY_LEFT_SPOILED;

    if (!CanaryCheck(&(stack->rightCanary)))
        return STACK_CANARY_RIGHT_SPOILED;

    if (!CanaryCheck((canary_t*) stack->dataBuffer))
        return DATA_CANARY_LEFT_SPOILED;

    if (!CanaryCheck((canary_t*) ((char*) stack->dataBuffer + sizeof(canary_t) +
                                          stack->bufferCapacity * stack->elemSize)))
        return DATA_CANARY_RIGHT_SPOILED;

    return OK;
}
#endif // CANARY_SWITCH_OFF


//----------------------------------------------------------------------------------------


#ifndef HASH_SWITCH_OFF

/**
 * 
 */
static hashData_t StackGetStructHash(Stack* stack)
{
    hashData_t structHashCopy = stack->structHash;
    hashData_t dataHashCopy   = stack->dataHash;
    StackHashDelete(stack);

    hashData_t structHash = 0;

    DoHash(&structHash, (hashData_t*) stack, sizeof(Stack));

    stack->structHash = structHashCopy;
    stack->dataHash   = dataHashCopy;

    return structHash;
}


/**
 * 
 */
static hashData_t StackGetDataHash(Stack* stack)
{
    hashData_t structHashCopy = stack->structHash;
    hashData_t dataHashCopy   = stack->dataHash;
    StackHashDelete(stack);

    hashData_t dataHash = 0;

    DoHash(&dataHash, (hashData_t*) stack->dataBuffer, 
           CANARY(2 * sizeof(canary_t) + ) stack->bufferCapacity * stack->elemSize);

    stack->structHash = structHashCopy;
    stack->dataHash   = dataHashCopy;

    return dataHash;
}


/**
 * 
 */
static void StackHashSet(Stack* stack)
{
    stack->structHash = StackGetStructHash(stack);
    stack->dataHash   = StackGetDataHash(stack);
}


/**
 * 
 */
static void StackHashDelete(Stack* stack)
{
    stack->structHash = 0;
    stack->dataHash   = 0;
}


/**
 * 
 */
static stackError_t StackCheckHash(Stack* stack)
{
    hashData_t structHash = StackGetStructHash(stack);
    if (structHash != stack->structHash)
        return STACK_STRUCT_HASH_WRONG;

    hashData_t dataHash = StackGetDataHash(stack);
    if (dataHash != stack->dataHash)
        return STACK_DATA_HASH_WRONG;

    return OK;
}


static void DoHash(hashData_t* hashBuffer, 
                   hashData_t* dataPtr, const size_t dataByteCount)
{
    #ifdef CRC32

    CRC32_Hash(hashBuffer, dataPtr, dataByteCount);

    #else

    #ifdef MURMUR32
    MURMUR32_Hash(hashBuffer, dataPtr, dataByteCount);
    #endif // MURMUR32

    #endif // CRC32
}

#endif // HASH_SWITCH_OFF
