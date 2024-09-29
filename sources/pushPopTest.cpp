#include <stdlib.h>
#include <time.h>

#include "../headers/stack.h"
#include "../logPrinter/logPrinter.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
static const size_t stackSize            = 10;
static long long    stackTest[stackSize] = {};


//----------------------------------------------------------------------------------------


/**
 * 
 */
static bool StackContentCreate();


/**
 * 
 */
static bool DoStackPush();


/**
 * 
 */
static bool DoStackPop();


//----------------------------------------------------------------------------------------


int main() 
{
    if (StackContentCreate() &&
        DoStackPush()        &&
        DoStackPop())
    {
        ColoredPrintf(GREEN, "Test's complete.\n");
    }
    else    
        ColoredPrintf(RED, "Test's not complete.\n");

    return 0;
}


//----------------------------------------------------------------------------------------


static bool StackContentCreate()
{


    return true;
}


static bool DoStackPush()
{


    return true;
}


static bool DoStackPop()
{


    return false;
}
