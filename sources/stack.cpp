#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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
struct Stack 
{
    void*  dataBuffer;    /**< Pointer to buffer with stack's data.        */
    size_t bufferSize;    /**< Max count of elems which stack can contain. */
    size_t elemSize;      /**< Size of each elem in bytes.                 */
    size_t elemCount;     /**< Count of elems in stack.                    */
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


//----------------------------------------------------------------------------------------


stackError_t StackInit(Stack** stack, const size_t elemSize)
{
    if (StackIsInit(*stack))
        return IS_INIT;

    *stack = (Stack*) calloc(1, sizeof(Stack));
    if (stack == NULL)
        return ALLOCATE_ERROR;
    
    (*stack)->bufferSize = MIN_STACK_SIZE;
    (*stack)->elemCount  = 0;
    (*stack)->elemSize   = elemSize;
    (*stack)->dataBuffer = calloc(MIN_STACK_SIZE, elemSize);

    if ((*stack)->dataBuffer == NULL)
    {
        StackDelete(stack);
        return ALLOCATE_ERROR;
    }

    return OK;
}


stackError_t StackDelete(Stack** stack)
{
    if (!StackIsInit(*stack))
        return NOT_INIT;
    
    free((*stack)->dataBuffer);
    (*stack)->dataBuffer = NULL;
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
