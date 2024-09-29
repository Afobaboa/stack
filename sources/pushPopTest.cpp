#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "../headers/stack.h"
#include "../logPrinter/logPrinter.h"


//----------------------------------------------------------------------------------------


/**
 * 
 */
struct StackPushPopTest
{
    size_t elemCount;   /**<  */
    size_t elemSize;    /**<  */
    void*  stackData;   /**<  */
    Stack* stack;       /**<  */
};


//----------------------------------------------------------------------------------------


/**
 * 
 */
static bool StackContentCreate(StackPushPopTest* stackTest, const size_t elemCount,
                                                            const size_t elemSize);


/**
 * 
 */
static bool StackContentDelete(StackPushPopTest* stackTest);


/**
 * 
 */
static bool DoStackPush(StackPushPopTest* stackTest);


/**
 * 
 */
static bool DoStackPop(StackPushPopTest* stackTest);


/**
 * 
 */
static bool StackPushPopTestPrint(StackPushPopTest* stackTest);


/**
 * 
 */
static bool AreElemsEqual(void* firstElemPtr, void* secondElemPtr, const size_t elemSize);


//----------------------------------------------------------------------------------------


int main() 
{
    StackPushPopTest stackTest = {};

    if (StackContentCreate(&stackTest, 10, 8) &&
        DoStackPush(&stackTest)               &&
        DoStackPop(&stackTest))
    {
        ColoredPrintf(GREEN, "%s is complete.\n", __FILE__);
    }
    else    
        ColoredPrintf(RED, "%s isn't complete.\n", __FILE__);

    return 0;
}


//----------------------------------------------------------------------------------------


static bool StackContentCreate(StackPushPopTest* stackTest, const size_t elemCount,
                                                            const size_t elemSize)
{
    stackTest->elemCount = elemCount;
    stackTest->elemSize  = elemSize;

    stackTest->stackData = calloc(elemCount, elemSize);
    if (stackTest->stackData == NULL)
    {
        LOG_PRINT(ERROR, "Memory can't be allocated. elemCount = %zu, elemSize = %zu.",
                                                     elemCount,       elemSize);
        return false;
    }
    
    srand((unsigned int) time(NULL));
    unsigned char* stackDataCopy = (unsigned char*) (stackTest->stackData);
    for (size_t elemNum = 0; elemNum < elemCount; elemNum++)
    {
        for (size_t byteNum = 0; byteNum < elemSize; byteNum++)
        {
            *stackDataCopy = (char) (rand() % UCHAR_MAX);
            stackDataCopy++;
        }
    }

    stackError_t stackError = STACK_CREATE(stackTest->stack, elemSize);
    if (stackError != OK)
    {
        LOG_PRINT(ERROR, "Stack can't be created, stackError code = %s.", 
                                                        GetStackErrorCode(stackError));
        return false;
    }

    return true;
}


static bool DoStackPush(StackPushPopTest* stackTest)
{
    char*  stackDataCopy = (char*) stackTest->stackData; 
    Stack* stack         =         stackTest->stack;
    size_t elemSize      =         stackTest->elemSize;

    for (size_t elemNum = 0; elemNum < stackTest->elemCount; elemNum++)
    {
        stackError_t stackError = StackPush(stack, stackDataCopy);
        if (stackError != OK)
        {
            LOG_PRINT(ERROR, "Elem [%zu] can't be pushed, stack error code = %s.",
                                                          GetStackErrorCode(stackError));
            return false;
        }

        stackDataCopy += elemSize;
    }

    return true;
}


static bool DoStackPop(StackPushPopTest* stackTest)
{
    size_t elemSize         =         stackTest->elemSize;
    size_t elemCountReduced =         stackTest->elemCount - 1;
    char*  stackDataEndCopy = (char*) stackTest->stackData + 
                                                        elemCountReduced * elemSize + 1; 
    Stack* stack            =         stackTest->stack;

    void* elemBuffer = NULL;
    elemBuffer = calloc(1, elemSize);
    if (elemBuffer == NULL)
    {
        LOG_PRINT(ERROR, "elemBuffer can't be allocated, elemSize = %zu.", elemSize);
        return false;
    }

    for (size_t elemNum = elemCountReduced; elemNum >= 0; elemNum++)
    {
        stackError_t stackError = StackPop(stack, elemBuffer);
        if (stackError != OK)
        {
            LOG_PRINT(ERROR, "Elem [%zu] can't be popped, stack error code = %s.",
                                                        GetStackErrorCode(stackError));
            return false;
        }


    }

    return true;
}


static bool StackPushPopTestPrint(StackPushPopTest* stackTest)
{


    return true;
}


static bool StackContentDelete(StackPushPopTest* stackTest)
{
    if (stackTest == NULL)
    {
        LOG_PRINT(ERROR, "stackTest == NULL.");
        return false;
    }

    free(stackTest->stackData);
    stackTest->stackData = NULL;

    stackTest->elemCount = 0;
    stackTest->elemSize  = 0;

    if (StackDelete(&(stackTest->stack)) != OK)
    {
        LOG_PRINT(ERROR, "Stack can't be deleted.");
        return false;
    }

    return true;
}


static bool AreElemsEqual(void* firstElemPtr, void* secondElemPtr, const size_t elemSize)
{
    for (size_t byteNum = 0; byteNum < elemSize; byteNum++)
    {
        if (*((char*) firstElemPtr + byteNum) != *((char*) secondElemPtr + byteNum))
            return false;
    }

    return true;
}
