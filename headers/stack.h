#ifndef STACK_H
#define STACK_H


//----------------------------------------------------------------------------------------


#include "../logPrinter/logPrinter.h"


//----------------------------------------------------------------------------------------


#define LOG_INIT


/**
 * 
 */
struct Stack;


//----------------------------------------------------------------------------------------


/**
 * 
 */
enum STACK_ERRORS 
{
    OK,             /**<  */
    IS_INIT,        /**<  */
    NOT_INIT,       /**<  */
    ALLOCATE_ERROR, /**<  */
    UNDERFLOW       /**<  */
};
typedef enum STACK_ERRORS stackError_t;


//----------------------------------------------------------------------------------------


/**
 * 
 */
stackError_t StackInit(Stack** stack, const size_t elemSize); // FIXME: StackCreate


/**
 * 
 */
stackError_t StackDelete(Stack** stack);


/**
 * 
 */
stackError_t StackPop(Stack* stack, void* elemBufferPtr);


/** 
 * 
 */
stackError_t StackPush(Stack* stack, void* elemPtr);


/**
 * 
 */
void StackDump(Stack** stack);


//----------------------------------------------------------------------------------------


#endif // STACK_H