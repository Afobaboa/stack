#include <stdlib.h>

#include "../logPrinter/logPrinter.h"
#include "../headers/canary.h"
#include "../headers/stack.h"

#include <stdio.h>


//----------------------------------------------------------------------------------------


/**
 * 
 */
#define CHECK_PUSH_POP_RETURNED_ERROR(stack, expectedStackError)      \
    CheckPushPopReturnedError(GET_PLACE(), stack, expectedStackError)


//----------------------------------------------------------------------------------------


/** 
 * 
 */
static bool StackLeftCanaryTest();


/**
 * 
 */
static bool StackRightCanaryTest();


/**
 * 
 */
static bool DataLeftCanaryTest();


/**
 * 
 */
static bool DataRightCanaryTest();


/**
 * 
 */
static bool CheckPushPopReturnedError(Place place,
                                      Stack* stack, stackError_t expectedStackError);


//----------------------------------------------------------------------------------------


int main() 
{
    #ifndef CANARY_SWITCH_OFF
    LOG_OPEN();

    if (StackLeftCanaryTest()  &&
        StackRightCanaryTest() &&
        DataLeftCanaryTest()   &&
        DataRightCanaryTest())
    {
        ColoredPrintf(GREEN, "Canary test is complete.\n");
    }
    else 
    {
        ColoredPrintf(RED, "Canary test isn't complete.\n");
    }

    LOG_CLOSE();
    
    #else
    ColoredPrintf(RED, "CANARY_SWITCH_OFF is defined1\n");
    #endif // CANARY_SWITCH_OFF

    return 0;
}


//----------------------------------------------------------------------------------------


static bool StackLeftCanaryTest()
{
    Stack* stack = NULL;
    STACK_CREATE(stack, 1);

    *((char*) stack) = 'f';

    if (!CHECK_PUSH_POP_RETURNED_ERROR(stack, STACK_CANARY_LEFT_SPOILED))
    {
        StackDelete(&stack);
        return false;
    }

    StackDelete(&stack);
    return true;
}


static bool StackRightCanaryTest()
{
    Stack* stack = NULL;
    STACK_CREATE(stack, 1);

    *((char*) stack + sizeof(canary_t) + sizeof(void*) + 3 * sizeof(size_t) 
                                                     ON_DEBUG( + sizeof(void*))) = 'f';
    
    if (!CHECK_PUSH_POP_RETURNED_ERROR(stack, STACK_CANARY_RIGHT_SPOILED))
    {
        StackDelete(&stack);
        return false;
    }

    StackDelete(&stack);
    return true;
}


static bool DataLeftCanaryTest()
{  
    Stack* stack = NULL;
    STACK_CREATE(stack, 1);

    char* dataBuffer = *((char**) ((char*) stack + sizeof(canary_t)));
    *dataBuffer = 'f';

    if (!CHECK_PUSH_POP_RETURNED_ERROR(stack, DATA_CANARY_LEFT_SPOILED))
    {
        StackDelete(&stack);
        return false;
    }

    StackDelete(&stack);
    return true;
}


static bool DataRightCanaryTest()
{
    Stack* stack = NULL;
    STACK_CREATE(stack, 1);

    char* dataBuffer = *((char**) ((char*) stack + sizeof(canary_t)));
    *(dataBuffer + sizeof(canary_t)) = 'f';

    if (!CHECK_PUSH_POP_RETURNED_ERROR(stack, DATA_CANARY_RIGHT_SPOILED))
    {
        StackDelete(&stack);
        return false;
    }

    StackDelete(&stack);
    return true;
}


static bool CheckPushPopReturnedError(Place place, 
                                      Stack* stack, stackError_t expectedStackError)
{
    char         elem       = 'f';
    stackError_t stackError = OK;

    stackError = StackPush(stack, &elem);
    if (stackError != expectedStackError)
    {
        LogPrint(ERROR, place, "Push(): stackError = %s\n"
                               "\texpectedStackError = %s\n", 
                                StackGetErrorName(stackError), 
                                StackGetErrorName(expectedStackError));
        return false;
    }

    stackError = StackPop(stack, &elem);
    if (stackError != expectedStackError)
    {
        LogPrint(ERROR, place,  "Pop():  stackError = %s\n"
                                "\texpectedStackError = %s\n", 
                                StackGetErrorName(stackError),
                                StackGetErrorName(expectedStackError));
        return false;
    }

    return true;
}
