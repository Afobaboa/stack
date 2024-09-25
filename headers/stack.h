#ifndef STACK_H
#define STACK_H


//----------------------------------------------------------------------------------------


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
    ALLOCATE_ERROR  /**<  */
};
typedef enum STACK_ERRORS stackError_t;


//----------------------------------------------------------------------------------------


/**
 * 
 */
stackError_t StackInit(Stack** stack);


/**
 * 
 */
stackError_t StackDelete(Stack** stack);


/**
 * 
 */
stackError_t StackPop(Stack* stack, void* elemBuffer);


/** 
 * 
 */
stackError_t StackPush(Stack* stack, void* elemPtr, const size_t elemSize);


//----------------------------------------------------------------------------------------


#endif // STACK_H