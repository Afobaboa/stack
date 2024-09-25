#include <stddef.h>
#include <stdlib.h>

#include "../headers/stack.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
struct Stack 
{
    void*  dataBuffer;    /**< Pointer to buffer with stack's data.     */
    size_t bufferSize;    /**< Size of stask's buffer in bytes.         */
    size_t takedSize;     /**< Size of taked memory of buffer in bytes. */
};


//----------------------------------------------------------------------------------------


/**
 * Minimum stack's bufferSize.
 */
static const size_t MIN_STACK_SIZE = 1024;


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


stackError_t StackInit(Stack** stack)
{
    if (StackIsInit(*stack))
        return IS_INIT;

    *stack = (Stack*) calloc(1, sizeof(Stack));
    if (stack == NULL)
        return ALLOCATE_ERROR;
    
    (*stack)->bufferSize = MIN_STACK_SIZE;
    (*stack)->takedSize  = 0;
    (*stack)->dataBuffer = calloc(MIN_STACK_SIZE, sizeof(char));

    if ((*stack)->dataBuffer == NULL)
        return ALLOCATE_ERROR;

    return OK;
}


stackError_t StackDelete(Stack** stack)
{
    if (!StackIsInit(*stack))
        return NOT_INIT;
    
    free((*stack)->dataBuffer);
    (*stack)->dataBuffer = NULL;
    (*stack)->bufferSize = 0;
    (*stack)->takedSize  = 0;

    free(*stack);
    *stack = NULL;

    return OK;
}


stackError_t StackPop(Stack* stack, void* elemBuffer)
{

}


stackError_t StackPush(Stack* stack, void* elemPtr, const size_t elemSize)
{

}


//----------------------------------------------------------------------------------------


static bool StackIsInit(Stack* stack)
{
    if (stack == NULL)
        return false;

    if (stack->dataBuffer == NULL &&
        stack->bufferSize == 0    &&
        stack->takedSize  == 0)
    {
        return false;
    }

    return true;
}


static bool StackIsExpandNeed(Stack* stack)
{
    if (stack->takedSize == stack->bufferSize)
        return true;

    return false;
}


static bool StackIsCompressNeed(Stack* stack)
{
    if (stack->bufferSize / 3 >= stack->takedSize || 
        stack->bufferSize     >= 2 * MIN_STACK_SIZE)
    {
        return true;
    }

    return false;
}


static stackError_t StackExpand(Stack* stack)
{                        // FIXME: new memory isn't initialized
    void* newDataBuffer = realloc(stack->dataBuffer, stack->bufferSize * 2); 
    if (newDataBuffer == NULL)
        return ALLOCATE_ERROR;

    if (stack->dataBuffer != newDataBuffer)
    {
        free(stack->dataBuffer);
        stack->dataBuffer = newDataBuffer;
    }
    stack->bufferSize *= 2;

    return OK;
}


static stackError_t StackCompress(Stack* stack)
{
    void* newDataBuffer = realloc(stack->dataBuffer, stack->bufferSize / 2); 
    if (newDataBuffer == NULL)
        return ALLOCATE_ERROR;

    if (stack->dataBuffer != newDataBuffer)
    {
        free(stack->dataBuffer);
        stack->dataBuffer = newDataBuffer;
    }
    stack->bufferSize /= 2;

    return OK;
}
