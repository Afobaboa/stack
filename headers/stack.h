#ifndef STACK_H
#define STACK_H


//----------------------------------------------------------------------------------------


#include "../headers/stackConfigs.h"
#include "../logPrinter/logPrinter.h"


//----------------------------------------------------------------------------------------

#ifndef DEBUG_SWITCH_OFF


/**
 * 
 */
#define STACK_INFO_GET(stack)                   \
    StackInfoGet(GET_NAME(stack), GET_PLACE())  


/**
 * 
 */
#define STACK_DUMP(stack)           \
    StackDump(stack, GET_PLACE())   


#else


/**
 * 
 */
#define STACK_INFO_GET(stack) 
#define STACK_DUMP(stack) 


#endif // DEBUG_SWITCH_OFF


//----------------------------------------------------------------------------------------


/**
 * 
 */
#define STACK_CREATE(stack, elemSize)                      \
    StackCreate(&stack, STACK_INFO_GET(stack), elemSize)   



//----------------------------------------------------------------------------------------


/**
 * 
 */
struct Stack;


/**
 * 
 */
ON_DEBUG(struct StackInfo;)


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
    UNDERFLOW,      /**<  */
    NULL_STACK_PTR  /**<  */
};
typedef enum STACK_ERRORS stackError_t;


//----------------------------------------------------------------------------------------


/**
 * 
 */
stackError_t StackCreate(Stack** stack, StackInfo* stackInfo, const size_t elemSize);


/**
 * 
 */
ON_DEBUG(StackInfo* StackInfoGet(const char* stackName, const Place place);)


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
ON_DEBUG(void StackDump(Stack* stack, Place place);)


//----------------------------------------------------------------------------------------


#endif // STACK_H