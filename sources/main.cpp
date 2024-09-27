#include <stdio.h>

#include "../headers/stack.h"


//----------------------------------------------------------------------------------------


int main() 
{
    LOG_OPEN();

    Stack* stack = NULL;
    STACK_CREATE(stack, sizeof(int));

    STACK_DUMP(stack);

    const size_t elemCount  = 5;
    stackError_t stackError = OK;
    int          elemBuffer = 0;

    for (size_t elemNum = 0; elemNum < elemCount; elemNum++)
    {
        printf("Enter %zu elem value\n", elemNum);
        scanf("%d", &elemBuffer);

        stackError = StackPush(stack, &elemBuffer);
        if (stackError != OK)
            return 1;
    }

    STACK_DUMP(stack);

    for (size_t elemNum = 0; elemNum < elemCount; elemNum++)
    {
        stackError = StackPop(stack, &elemBuffer);
        if (stackError != OK)
            return 1;

        printf("%zu elem value is %d\n", elemNum, elemBuffer);
    }

    STACK_DUMP(stack);
    StackDelete(&stack);
    LOG_CLOSE();
    return 0;
}