#include <stdio.h>

#include "../headers/stack.h"


//----------------------------------------------------------------------------------------


int main() 
{
    Stack* stack = NULL;
    STACK_CREATE(stack, sizeof(int));

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

    for (size_t elemNum = 0; elemNum < elemCount; elemNum++)
    {
        stackError = StackPop(stack, &elemBuffer);
        if (stackError != OK)
            return 1;

        printf("%zu elem value is %d\n", elemNum, elemBuffer);
    }

    StackDelete(&stack);
    return 0;
}