/**
 * @file 
 * This header provides you dynamic stack.
 * You can push and pop elems of arbitrary size into this stack.
 * This stack has canary and hash data defence, of you don't switched it off.
 * 
 * Use Stack* type to declary stack. Don't forget init Stack* by NULL.
 * Dont't change and use data at Stack* yourself. It's UB. Use this API instead.
 * 
 * Use only macros STACK_CREATE to create the stack. 
 * Use StackPush() and StackPop() to push and pop elems.
 * Don't forget delete stack by StackDelete().
 * 
 * Use only macros STACK_DUMP() to print all info about stack and it's data to log file.
 */

#ifndef STACK_H
#define STACK_H


//----------------------------------------------------------------------------------------


#include "../headers/stackConfigs.h"
#include "../logPrinter/logPrinter.h"


//----------------------------------------------------------------------------------------

#ifndef DEBUG_SWITCH_OFF


/**
 * DON'T USE IT IN YOUR PROGRAM.
 * 
 * This macros return you ptr to info about your stack.
 * 
 * @param stack Name of stack's (Stack*) variable.
 */
#define STACK_INFO_GET(stack)                   \
    StackInfoGet(GET_NAME(stack), GET_PLACE())  


/**
 * This definition provides convinient way to get sack's info.
 * This info is needed by StackDump.
 * 
 * @param stack Your Stack* stack.
 */
#define STACK_DUMP(stack)           \
    StackDump(stack, GET_PLACE())   


#else


/**
 * Definitions if DEBUG_SWITCH_OFF
 */
#define STACK_INFO_GET(stack) 
#define STACK_DUMP(stack) 


#endif // DEBUG_SWITCH_OFF


//----------------------------------------------------------------------------------------


/**
 * This definitions create stack and get some information about it's creating.
 * 
 * @param stack    Your Stack* stack you want to create.
 * @param elemSize Size of each elem of stack in bytes.
 */
#define STACK_CREATE(stack, elemSize)                      \
    StackCreate(&stack, ON_DEBUG(STACK_INFO_GET(stack),) elemSize)   



//----------------------------------------------------------------------------------------


/**
 * A convinient way to avoid problems with type checking.
 */
#ifndef DEBUG_SWITCH_OFF
struct StackInfo;
#endif // DEBUG_SWITCH_OFF


/**
 * A convinient way to avoid problems with type checking.
 * Use Stack* in your program.
 */
struct Stack;


//----------------------------------------------------------------------------------------


/**
 * Stack error's codes. You can use it to get information of errors.
 */
enum STACK_ERRORS 
{
    OK,                         /**< All is OK.                                        */
    IS_INIT,                    /**< You are trying create stack whic is alreade init. */
    ALLOCATE_ERROR,             /**< Error with allocating memory.                     */
    UNDERFLOW,                  /**< You are trying pop elem from stach with 0 elems.  */
    STACK_NULL_PTR,             /**< You hands over NULL ptr to stack into funtion.    */
    BUFFER_NULL_PTR,            /**< Ptr to stack's buffer is NULL.                    */
    CAPACITY_OVERFLOW,          /**< Too large capaity was achieved.                   */
    ELEM_SIZE_OVERFLOW,         /**< You are trying to set too large elem's size.      */
    ELEM_COUNT_OVERFLOW         /**< Too large elem's count was achieved.              */
    
    #ifndef DEBUG_SWITCH_OFF
    ,STACK_INFO_NULL_PTR        /**< Ptr to stack's info is NULL.                      */
    ,STACK_INFO_NULL_NAME       /**< Ptr to stack's name is NULL.                      */
    ,STACK_INFO_WRONG_PLACE     /**< Place of stack's creating wasn't set correctly.   */
    #endif // DEBUG_SWITCH_OFF

    #ifndef CANARY_SWITCH_OFF
    ,STACK_CANARY_RIGHT_SPOILED /**< Right canary of struct Stack was spoiled.         */
    ,STACK_CANARY_LEFT_SPOILED  /**< Left canary of struct Stack was spoiled.          */
    ,DATA_CANARY_RIGHT_SPOILED  /**< Right canary of stack's buffer was spoiled.       */
    ,DATA_CANARY_LEFT_SPOILED   /**< Left canary of stack's buffer was spoiled.        */
    #endif // CANARY_SWITCH_OFF

    #ifndef HASH_SWITCH_OFF
    ,STACK_STRUCT_HASH_WRONG    /**< Hash of struct Stack is wrong.                    */
    ,STACK_DATA_HASH_WRONG      /**< Hash of stack's buffer's data is wrong.           */
    #endif // HASH_SWITCH_OFF
};
typedef enum STACK_ERRORS stackError_t;


//----------------------------------------------------------------------------------------


/**
 * DON'T USE IT IN YOUR PROGRAM.
 * Insted of this, use STACK_CREATE macros.
 * 
 * This function create Stack and set some useful information of stack.
 * 
 * @param stack     Ptr to your Stack* stack.
 * @param stackInfo Info about stack. That's using only in debug version.
 * @param elemSize  Size of each elem of stack in bytes.
 * 
 * @return Stack error status.
 */
stackError_t StackCreate(Stack**      stack, ON_DEBUG(StackInfo* stackInfo,) 
                         const size_t elemSize);


/**
 * DONT'T USE IT IN YOUR PROGRAM.
 * This function has convinient macros to use.
 * 
 * This function provides you get StackInfo* about your stack.
 * 
 * @param stackName Name of your stack.
 * @param place     Place of using this function.
 * 
 * @return Ptr to struct with info about stack.
 */
#ifndef DEBUG_SWITCH_OFF
StackInfo* StackInfoGet(const char* stackName, const Place place);
#endif // DEBUG_SWITCH_OFF


/**
 * Delete all stack's data.
 * 
 * @param stack Ptr to your Stack* stack.
 * 
 * @return Stack error status.
 */
stackError_t StackDelete(Stack** stack);


/**
 * Pop elem from stack.
 * 
 * @param stack         Your Stack* stack.
 * @param elemBufferPtr Ptr to buffer where will be written value of element.
 *                      If buffer's size won't be equal to stack's elemSize, 
 *                      that will be UB.
 * 
 * @return Stack error status.
 */
stackError_t StackPop(Stack* stack, void* elemBufferPtr);


/** 
 * Push elem to stack.
 * 
 * @param stack   Your Stack* stack.
 * @param elemPtr Ptr to buffer which contains content of elem.
 *                If buffer's size won't be equal to stack's elemSize, that will be UB.
 * 
 * @return Stack error status.
 */
stackError_t StackPush(Stack* stack, void* elemPtr);


/**
 * DONT'T USE IT IN YOUR PROGRAM.
 * Use macros STACK_DUMP() unstead.
 * 
 * This function prints all information about stack.
 * 
 * @param stack Your Stack* stack.
 * @param place Place of this function's using.
 */
#ifndef DEBUG_SWITCH_OFF
void StackDump(Stack* stack, Place place);
#endif // DEBUG_SWITCH_OFF


/**
 * This finction prvides you to get name of all stack errors.
 * 
 * @param stackError_t Stack error status.
 * 
 * @return Ptr to name of your stack error status.
 */
const char* StackGetErrorName(const stackError_t stackError);


//----------------------------------------------------------------------------------------


#endif // STACK_H