#include <stdlib.h>

#include "../logPrinter/logPrinter.h"
#include "../headers/canary.h"
#include "../headers/stack.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
static void StackSpoileLeftCanary(Stack* stack);


//----------------------------------------------------------------------------------------


int main() 
{
    #ifndef CANARY_SWITCH_OFF
    LOG_OPEN();

    Stack* stack = NULL;
    STACK_CREATE(stack, sizeof(int));
    StackSpoileLeftCanary(stack);
    // int* dataLeftCanary = *((int**) ((char*) stack + sizeof(canary_t)));
    // *dataLeftCanary = 666;

    int          elem       = 666;
    stackError_t stackError = OK;

    stackError = StackPush(stack, &elem);
    if (stackError != OK)
        ColoredPrintf(RED, "Push(): stackError = %s\n", StackGetErrorName(stackError));

    stackError = StackPop(stack, &elem);
    
    if (stackError != OK)
        ColoredPrintf(RED, "Pop(): stackError = %s\n", StackGetErrorName(stackError));

    ColoredPrintf(YELLOW, "test's done.\n");

    STACK_DUMP(stack);
    StackDelete(&stack);
    LOG_CLOSE();
    
    #else
    ColoredPrintf(RED, "CANARY_SWITCH_OFF is defined1\n");
    #endif // CANARY_SWITCH_OFF

    return 0;
}


//----------------------------------------------------------------------------------------


static void StackSpoileLeftCanary(Stack* stack)
{
    *((char*) stack + 5 /*48*/) = 'c'; 
}
