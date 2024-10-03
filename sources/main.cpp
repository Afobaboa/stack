#include <stdio.h>
#include <stdlib.h>

#include "../logPrinter/logPrinter.h"
#include "../headers/stack.h"


//----------------------------------------------------------------------------------------


int main() 
{
    LOG_OPEN();

    Stack* stack = NULL;
    STACK_CREATE(stack, sizeof(int));

    STACK_DUMP(stack);

    const size_t elemCount  = 3;
    stackError_t stackError = OK;
    int          elemBuffer = 0;
    char*        format     = GET_ARRAY_PRINTING_FORMAT(elemCount);

    for (size_t elemNum = 0; elemNum < elemCount; elemNum++)
    {
        printf("Enter %zu elem value\n", elemNum);
        scanf("%d", &elemBuffer);

        stackError = StackPush(stack, &elemBuffer);
        if (stackError != OK)
        {
            LOG_PRINT(ERROR, "Stack error code = %s\n", StackGetErrorName(stackError));
            free(format);
            StackDelete(&stack);
            return 1;
        }
    }

    STACK_DUMP(stack);

    *((char*) stack + 5) = 'l';
    for (size_t elemNum = 0; elemNum < elemCount; elemNum++)
    {
        stackError = StackPop(stack, &elemBuffer);
        if (stackError != OK)
        {
            printf("stackError = %s\n", StackGetErrorName(stackError));
            free(format);
            StackDelete(&stack);
            return 1;
        }

        printf(format, elemNum);
        printf("%d\n", elemBuffer);
    }

    free(format);
    STACK_DUMP(stack);
    StackDelete(&stack);
    LOG_CLOSE();
    return 0;
}