#include <stdlib.h>
#include <time.h>

#include "../headers/stack.h"
#include "../logPrinter/logPrinter.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
static const size_t stackSize           = 10;
static long long    stackTest[stackSize] = {};


//----------------------------------------------------------------------------------------


/**
 * 
 */
static void StackContentCreate();


/**
 * 
 */
static void DoStackPush();


/**
 * 
 */
static void DoStackPop();


//----------------------------------------------------------------------------------------


int main() 
{
    StackContentCreate();
    DoStackPush();
    DoStackPop();

    ColoredPrintf(GREEN, "Test's complete.\n");

    return 0;
}


//----------------------------------------------------------------------------------------


static void StackContentCreate()
{

}


static void DoStackPush()
{

}


static void DoStackPop()
{

}
