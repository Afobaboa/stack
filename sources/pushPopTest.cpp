#include <stdlib.h>
#include <time.h>

#include "../headers/stack.h"
#include "../logPrinter/logPrinter.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
struct StackTest
{
    const size_t capacity;    /**<  */
    const size_t elemSize;    /**<  */
    void*        stackData;   /**<  */
    Stack*       stack;       /**<  */
};


//----------------------------------------------------------------------------------------


/**
 * 
 */
static bool StackContentCreate(StackTest* stackTest);


/**
 * 
 */
static bool DoStackPush(StackTest* stackTest);


/**
 * 
 */
static bool DoStackPop(StackTest* stackTest);


/**
 * 
 */
static bool StackTestPrint(StackTest* stackTest);


//----------------------------------------------------------------------------------------


int main() 
{
    StackTest stackTest = {.capacity  = 10,
                           .elemSize  = 8,
                           .stackData = NULL,
                           .stack     = NULL};

    if (StackContentCreate(&stackTest) &&
        DoStackPush(&stackTest)        &&
        DoStackPop(&stackTest))
    {
        ColoredPrintf(GREEN, "%s is complete.\n", __FILE__);
    }
    else    
        ColoredPrintf(RED, "%s isn't complete.\n", __FILE__);

    return 0;
}


//----------------------------------------------------------------------------------------


static bool StackContentCreate(StackTest* stackTest)
{
    srand((unsigned int) time(NULL));

    

    return true;
}


static bool DoStackPush(StackTest* stackTest)
{


    return true;
}


static bool DoStackPop(StackTest* stackTest)
{


    return true;
}


static bool StackTestPrint(StackTest* stackTest)
{


    return true;
}
