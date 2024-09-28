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
#define STACK_TRY_EXPAND(stack)                 \
    {                                           \
        if (StackIsExpandNeed(stack))           \
        {                                       \
            stackError_t stackError = OK;       \
            stackError = StackExpand(stack);    \
                                                \
            if (stackError != OK)               \
                return stackError;              \
        }                                       \
    }                                           \


/**
 * 
 */
#define STACK_TRY_COMPRESS(stack)               \
    {                                           \
        if (StackIsCompressNeed(stack))         \
        {                                       \
            stackError_t stackError = OK;       \
            stackError = StackCompress(stack);  \
                                                \
            if (stackError != OK)               \
                return stackError;              \
        }                                       \
    }                                           \


//----------------------------------------------------------------------------------------


/**
 * 
 */
struct StackInfo
{
    const char* name;   /**<  */
    Place       place;  /**<  */
};


/**
 * 
 */
struct Stack 
{
    void*      dataBuffer;    /**< Pointer to buffer with stack's data.        */
    size_t     bufferSize;    /**< Max count of elems which stack can contain. */
    size_t     elemSize;      /**< Size of each elem in bytes.                 */
    size_t     elemCount;     /**< Count of elems in stack.                    */

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
static void StackInfoPrint(StackInfo* stackInfo);


/**
 * 
 */
static void StackPrintContent(Stack* stack);


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
static void StackPrintELem(Stack* stack, const size_t elenNum);


/**
 * 
 */
static void StackPrintFields(Stack* stack);


//----------------------------------------------------------------------------------------


stackError_t StackCreate(Stack** stack, StackInfo* stackInfo, const size_t elemSize)
{
    if (stack == NULL)
        return NULL_STACK_PTR;

    if (StackIsInit(*stack))
        return IS_INIT;

    *stack = (Stack*) calloc(1, sizeof(Stack));
    // TODO: Add check
    (*stack)->bufferSize = MIN_STACK_SIZE;
    (*stack)->elemCount  = 0;
    (*stack)->elemSize   = elemSize;
    (*stack)->stackInfo  = stackInfo;
    (*stack)->dataBuffer = calloc(MIN_STACK_SIZE, elemSize);

    if ((*stack)->dataBuffer == NULL)
    {
        StackDelete(stack);
        return ALLOCATE_ERROR;
    }

    return OK;
}


StackInfo* StackInfoGet(const char* stackName, const Place place)
{
    StackInfo* stackInfo = (StackInfo*) calloc(1, sizeof(StackInfo));

    stackInfo->name  = stackName;
    stackInfo->place = place;  

    return stackInfo;
}


stackError_t StackDelete(Stack** stack)
{
    if (stack == NULL)
        return NULL_STACK_PTR;

    if (!StackIsInit(*stack))
        return NOT_INIT;
    
    free((*stack)->dataBuffer);
    free((*stack)->stackInfo);

    (*stack)->dataBuffer = NULL;
    (*stack)->stackInfo  = NULL;
    (*stack)->bufferSize = 0;
    (*stack)->elemCount  = 0;
    (*stack)->elemSize   = 0;

    free(*stack);
    *stack = NULL;

    return OK;
}


stackError_t StackPop(Stack* stack, void* elemBufferPtr)
{
    if (stack->elemCount == 0)
        return UNDERFLOW;
    
    STACK_TRY_COMPRESS(stack);
    // TODO: fuck macros!!!

    // printf("bufferSize = %zu\n", stack->bufferSize);

    // printf("dataBuffer = %p\n", stack->dataBuffer);
    void* stackElemPtr = (char*) stack->dataBuffer + (stack->elemCount - 1) * 
                                                                        stack->elemSize;
    // printf("stackElemPtr = %p\n", stackElemPtr);
    // printf("stackElem value = %d\n\n", *((int*) stackElemPtr));

    // printf("elemBuffer value = %d\n\n", *((int*) elemBufferPtr));

    // *((int*) elemBufferPtr) = *((int*) stackElemPtr);
    if (memmove(elemBufferPtr, stackElemPtr, stack->elemSize) == NULL)
        return ALLOCATE_ERROR;

    if (memset(stackElemPtr, 0, stack->elemSize) == NULL)
        return ALLOCATE_ERROR;

    stack->elemCount -= 1;

    return OK;
}


stackError_t StackPush(Stack* stack, void* elemPtr)
{
    STACK_TRY_EXPAND(stack);

    // printf("dataBuffer = %p\n", stack->dataBuffer);
    void* stackElemPtr = (char*) stack->dataBuffer + stack->elemCount * stack->elemSize;
    // printf("stackElemPtr = %p\n", stackElemPtr);


    if (memmove(stackElemPtr, elemPtr, stack->elemSize) == NULL)
        return ALLOCATE_ERROR;

    // printf("stackElem value = %d\n\n", *((int*) stackElemPtr));

    stack->elemCount += 1;

    return OK;
}


void StackDump(Stack* stack, Place place) 
{
    LogPrint(INFO, place, "Stack's dumping...\n");

    StackPrintFields(stack);
    StackPrintContent(stack);
}


//----------------------------------------------------------------------------------------


static bool StackIsInit(Stack* stack)
{
    if (stack == NULL)
        return false;

    if (stack->dataBuffer == NULL &&
        stack->bufferSize == 0    &&
        stack->elemCount  == 0    &&
        stack->elemSize   == 0)
    {
        return false;
    }

    return true;
}


static bool StackIsExpandNeed(Stack* stack)
{
    if (stack->elemCount == stack->bufferSize)
        return true;

    return false;
}


static bool StackIsCompressNeed(Stack* stack)
{
    if (stack->bufferSize / 4 >= stack->elemCount &&
        stack->bufferSize     >= 2 * MIN_STACK_SIZE)
    {
        // printf("bufferSize = %zu, elemCount = %zu, min stack size = %zu\n",
                // stack->bufferSize, stack->elemCount, MIN_STACK_SIZE);
        return true;
    }

    return false;
}


static stackError_t StackExpand(Stack* stack)
{                       
                         // FIXME: add MyRecalloc()
    // void* newDataBuffer = realloc(stack->dataBuffer, stack->bufferSize * 2 * stack->elemSize); 
    void* newDataBuffer = MyRecalloc(stack->dataBuffer, stack->bufferSize, 
                                     stack->bufferSize * 2, stack->elemSize);
    if (newDataBuffer == NULL)
        return ALLOCATE_ERROR;

    if (stack->dataBuffer != newDataBuffer)
        stack->dataBuffer = newDataBuffer;

    stack->bufferSize *= 2;

    return OK;
}


static stackError_t StackCompress(Stack* stack)
{   
                         // FIXME: add MyRecalloc()
    // printf("stack->bufferSize = %zu\n", stack->bufferSize);
    void* newDataBuffer = MyRecalloc(stack->dataBuffer, stack->elemSize,
                                     stack->bufferSize / 2, stack->elemSize);
    // printf("newDataBuffer = %p\n", newDataBuffer);
    if (newDataBuffer == NULL)
        return ALLOCATE_ERROR;

    if (stack->dataBuffer != newDataBuffer)
        stack->dataBuffer = newDataBuffer;

    stack->bufferSize /= 2;

    return OK;
}


static void StackInfoPrint(StackInfo* stackInfo)
{
    LOG_DUMMY_PRINT("\tStack %s was created in %s: %s(): line %d\n\n",
                    stackInfo->name,           stackInfo->place.file, 
                    stackInfo->place.function, stackInfo->place.line);
}


static void StackPrintContent(Stack* stack)
{
    for (size_t elemNum = 0; elemNum < stack->elemCount; elemNum++)
    {
        LOG_DUMMY_PRINT("\t[%zu] = ", elemNum);
        StackPrintELem(stack, elemNum);
        LOG_DUMMY_PRINT("\n");
    }
    LOG_DUMMY_PRINT("\n");
}


static void StackPrintELem(Stack* stack, const size_t elemNum)
{
    const size_t elemSize  = stack->elemSize;
    const char* elemPtr = (char*) stack->dataBuffer + elemNum * elemSize;

    for (size_t byteNum = 0; byteNum < elemSize; byteNum++)
    {
        LOG_DUMMY_PRINT("%hu", (unsigned short) *(elemPtr + byteNum));
    }
}


static void StackPrintFields(Stack* stack)
{
    StackInfoPrint(stack->stackInfo);

    LOG_DUMMY_PRINT("\tdataBuffer = %p \n"
                    "\tbufferSize = %zu \n"
                    "\telemSize = %zu \n"
                    "\telemCount = %zu \n\n",
                    stack->dataBuffer, 
                    stack->bufferSize,
                    stack->elemSize,
                    stack->elemCount);
}