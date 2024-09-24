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
static void StackExpand(Stack* stack);


/**
 * 
 */
static void StackCpmpress(Stack* stack);


//----------------------------------------------------------------------------------------


stackError_t StackInit(Stack* stack)
{
    if (StackIsInit(stack))
        return IS_INIT;
    
    stack->bufferSize = MIN_STACK_SIZE;
    stack->takedSize  = 0;
    stack->dataBuffer = (void*) calloc(MIN_STACK_SIZE, sizeof(char));

    return OK;
}


stackError_t StackDelete(Stack* stack)
{
    if (!StackIsInit(stack))
        return NOT_INIT;
    
    free(stack->dataBuffer);

    stack->dataBuffer = NULL;
    stack->bufferSize = 0;
    stack->takedSize  = 0;

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
    if (stack->dataBuffer == NULL &&
        stack->bufferSize == 0    &&
        stack->takedSize  == 0)
    {
        return false;
    }

    return true;
}


static void StackExpand(Stack* stack)
{

}


static void StackCpmpress(Stack* stack)
{

}
