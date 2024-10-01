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
    StackCreate(&stack, ON_DEBUG(STACK_INFO_GET(stack),) elemSize)   



//----------------------------------------------------------------------------------------


/**
 * 
 */
struct Stack;


/**
 * 
 */
#ifndef DEBUG_SWITCH_OFF
struct StackInfo;
#endif // DEBUG_SWITCH_OFF


//----------------------------------------------------------------------------------------


/**
 * 
 */
enum STACK_ERRORS 
{
    OK,                     /**<  */
    IS_INIT,                /**<  */
    ALLOCATE_ERROR,         /**<  */
    UNDERFLOW,              /**<  */
    STACK_NULL_PTR,         /**<  */
    BUFFER_NULL_PTR,        /**<  */
    CAPACITY_OVERFLOW,      /**<  */
    ELEM_SIZE_OVERFLOW,     /**<  */
    ELEM_COUNT_OVERFLOW     /**<  */
    
    #ifndef DEBUG_SWITCH_OFF
    ,STACK_INFO_NULL_PTR    /**<  */
    ,STACK_INFO_NULL_NAME   /**<  */
    ,STACK_INFO_WRONG_PLACE /**<  */
    #endif // DEBUG_SWITCH_OFF
};
typedef enum STACK_ERRORS stackError_t;


//----------------------------------------------------------------------------------------


/**
 * 
 */
stackError_t StackCreate(Stack**      stack, ON_DEBUG(StackInfo* stackInfo,) 
                         const size_t elemSize);


/**
 * 
 */
#ifndef DEBUG_SWITCH_OFF
StackInfo* StackInfoGet(const char* stackName, const Place place);
#endif // DEBUG_SWITCH_OFF


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
#ifndef DEBUG_SWITCH_OFF
void StackDump(Stack* stack, Place place);
#endif // DEBUG_SWITCH_OFF


/**
 * 
 */
const char* StackGetErrorCode(const stackError_t stackError);


//----------------------------------------------------------------------------------------


#endif // STACK_H