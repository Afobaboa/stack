#include <stddef.h>
#include <stdlib.h>

#include "../headers/stack.h"


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


stackError_t StackInit(Stack* stack)
{
    if (StackIsInit(stack))
        return IS_INIT;
    
    stack->bufferSize  = MIN_STACK_SIZE;
    stack->takedSize   = 0;
    stack->dataBuffer  = (void*) calloc(MIN_STACK_SIZE, sizeof(char));
    stack->lastElemPtr = stack->dataBuffer;

    if (stack->dataBuffer == NULL)
        return ALLOCATE_ERROR;

    return OK;
}


stackError_t StackDelete(Stack* stack)
{
    if (!StackIsInit(stack))
        return NOT_INIT;
    
    free(stack->dataBuffer);
    free(stack->lastElemPtr);

    stack->dataBuffer  = NULL;
    stack->lastElemPtr = NULL;
    stack->bufferSize  = 0;
    stack->takedSize   = 0;

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
    if (stack->dataBuffer  == NULL &&
        stack->lastElemPtr == NULL &&
        stack->bufferSize  == 0    &&
        stack->takedSize   == 0)
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
