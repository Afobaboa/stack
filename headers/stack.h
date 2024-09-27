#ifndef STACK_H
#define STACK_H


//----------------------------------------------------------------------------------------


#include "../logPrinter/logPrinter.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
#define STACK_INFO_GET(stack)                   \
    StackInfoGet(GET_NAME(stack), GET_PLACE())  \


/**
 * 
 */
#define STACK_CREATE(stack, elemSize)                      \
    StackCreate(&stack, STACK_INFO_GET(stack), elemSize)   \


//----------------------------------------------------------------------------------------


/**
 * 
 */
struct Stack;


/**
 * 
 */
struct StackInfo;


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
stackError_t StackCreate(Stack** stack, StackInfo stackInfo, const size_t elemSize);


/**
 * 
 */
StackInfo StackInfoGet(const char* stackName, const Place place);


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