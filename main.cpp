#include "stack.h"
#include <limits.h>


int main()
{
    stack_t stk = {};
    stackElem value = {};
    size_t capacity = 1;
    stackCtor (&stk, capacity);
    /*stackPush(&stk, 200);
    stackPop(&stk, &value);
    stackDtor(&stk);*/

    return 0;
}
