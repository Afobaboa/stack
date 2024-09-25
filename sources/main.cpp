#include <stdio.h>

#include "../headers/stack.h"


//----------------------------------------------------------------------------------------


int main() 
{
    Stack* stack = NULL;
    StackInit(&stack);
    printf("Stack's init\n");

    StackDelete(&stack);
    printf("Stack's delete\n");

    return 0;
}