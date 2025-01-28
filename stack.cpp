#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "stack.h"

FILE* DUMP_FILE = fopen (dump_file, "w");

FILE* ERROR_FILE = fopen (error_file, "w");

stackErrors stackCtor (stack_t* stk, size_t capacity)
{
    assert (stk);
    if (capacity < 1) return ERR;
    stk->capacity = capacity;

    stk->data = (stackElem*)calloc(stk->capacity + 2, sizeof(stackElem));
    if (stk->data == nullptr) return DATA_EMPTY;
    stk->size = 0;
    for (size_t i = 0; i < capacity+2; i++)
        stk->data[i] = POISON;
    //NOTE выводить инфу о стеке после каждой операции (какая операция с номером), а ошибки отдельно

    STACK_CHECK(stk);

    //NOTE -  почему memcmp?
    //printf ("%p\npoison-%p\n", stk->data, &(POISON));

    return OK;
}

stackErrors stackDtor (stack_t* stk)
{
    STACK_CHECK(stk);

    free (stk->data);
    stk->data = nullptr;
    stk->size = INT_MAX;
    stk->capacity = INT_MAX;

    return OK;
}

stackErrors stackDump (stack_t* stk, const char* func)
{
    if (DUMP_FILE == nullptr) return FILE_ERR;

    PRINT_STARS_IN(DUMP_FILE, func);

    fprintf (DUMP_FILE, "stack adress - %p\n", stk);
    fprintf (DUMP_FILE, "data adress - %p\n", stk->data);
    fprintf (DUMP_FILE, "size - %zu (%zu)\n", stk->size, stk->size+1);
    fprintf (DUMP_FILE, "capacity - %zu (%zu)\n\n", stk->capacity, stk->capacity+2);
    /*fprintf (DUMP_FILE, "DATA_LEFT_CANARY IS - " SPEC "\n", stk->data[0]); значение канареек стека изменить нельзя в тч через адрес
    fprintf (DUMP_FILE, "DATA_RIGHT_CANARY IS - " SPEC "\n", stk->data[stk->capacity-1]);*/

    fprintf (DUMP_FILE, "\ndata[LEFT_CANARY]:"SPEC"\n\n", stk->data[0]);

    for (size_t i = 1 ; i < stk->size+1; i++)
    {
        fprintf (DUMP_FILE, "data[%zu]: Adress - %p, Value - " SPEC "\n",
                i, &(stk->data[i]), stk->data[i]);
    }

    fprintf (DUMP_FILE, "\ndata[size]:"SPEC"\n\n", stk->data[stk->size+1]);

    for (size_t i = stk->size+2; i < stk->capacity+1; i++)
    {
        fprintf (DUMP_FILE, "data[%zu]: Adress - %p, Value - " SPEC "\n",
                i, &(stk->data[i]), stk->data[i]);
    }

    fprintf (DUMP_FILE, "\ndata[RIGHT_CANARY]:"SPEC"\n", stk->data[stk->capacity+1]);

    PRINT_STARS_IN(DUMP_FILE, "*********");

    return OK;
}


stackErrors stackPush (stack_t* stk, stackElem value)
{
    STACK_CHECK(stk);

    stk->data[stk->size+1] = value;
    (stk->size)++;

    STACK_CHECK(stk);

    return OK;
}

stackErrors stackPop (stack_t* stk, stackElem* value)
{
    STACK_CHECK(stk);

    if (stk->size == 0) return SIZE_ERR;
    (stk->size)--;
    *value = stk->data[stk->size+1];

    STACK_CHECK(stk);

    return OK;
}

stackErrors stackVerify (stack_t* stk, const char* file, size_t line, const char* func)
{
    if (ERROR_FILE == nullptr) return FILE_ERR;

    bool is_any_errors = 0;
    //bool* errors = (bool*)calloc(8, sizeof(bool));

    PRINT_STARS_IN(ERROR_FILE, func);

    if (!stk)
    {
        is_any_errors = 1;
        PRINT_ERR
        fprintf (ERROR_FILE, "Stack = NULL\n");
        //errors[STK_NULL] = 1;
    }

    if (stk->LEFT_СANARY != POISON)
    {
        is_any_errors = 1;
        PRINT_ERR
        fprintf (ERROR_FILE, "Left canary hasn't POISON value\n");
        //errors[STK_CANARY_ERR] = 1;
    }

    if (stk->RIGHT_СANARY != POISON)
    {
        is_any_errors = 1;
        PRINT_ERR
        fprintf (ERROR_FILE, "Right canary hasn't POISON value\n");
        //errors[STK_CANARY_ERR] = 1;
    }

    if (!(stk->data))
    {
        is_any_errors = 1;
        PRINT_ERR
        fprintf (ERROR_FILE, "stk.data = NULL\n");
        //errors[DATA_EMPTY] = 1;
    }

    /*if (!memcmp (&(stk->data[0]), &(POISON), sizeof(POISON)))
    {
        PRINT_ERR
        fprintf (ERROR_FILE, "ERROR: Left canary in data has damaged\n");
        if (!memcmp (&(stk->data[stk->capacity]), &(POISON), sizeof(POISON)))
        {
            PRINT_ERR
            fprintf (ERROR_FILE, "ERROR: Right canary in data has damaged\n");
            return CANARY_ERR;
        }
        return CANARY_ERR;
    }

    if (!memcmp (&(stk->data[stk->capacity]), &(POISON), sizeof(POISON)))
    {
        PRINT_ERR
        fprintf (ERROR_FILE, "ERROR: Right canary in data has damaged\n");
        return CANARY_ERR;
    }*/

    if (stk->size == INT_MAX)
    {
        is_any_errors = 1;
        PRINT_ERR
        fprintf (ERROR_FILE, "wrong size\n");
        //errors[SIZE_ERR] = 1;
    }

    if (stk->capacity == INT_MAX || stk->capacity < 1)
    {
        is_any_errors = 1;
        PRINT_ERR
        fprintf (ERROR_FILE, "wrong capacity\n");
        //errors[CAPACITY_ERR] = 1;
    }

    if (stk->size > stk->capacity)
    {
        is_any_errors = 1;
        PRINT_ERR
        fprintf (ERROR_FILE, "size > capacity\n");
        //errors[SIZE_OVERFLOW] = 1;
    }

    bool flag_of_wrong_element = 1;

    for (size_t i = 1; i < stk->size; i++)
    {
        if (stk->data[i] == POISON)
        {
            fprintf (ERROR_FILE, "element [%zu] = POISON value\n", i);
            flag_of_wrong_element = 0;
        }
    }

    for (size_t i = stk->size; i < stk->capacity; i++)
    {
        if (stk->data[i] != POISON)
        {
            fprintf (ERROR_FILE, "element [%zu] != POISON value\n", i);
            flag_of_wrong_element = 0;
        }
    }
    if (!flag_of_wrong_element)
    {
        is_any_errors = 1;
        PRINT_ERR
        //errors[ELEMENT_ERR] = 1;
    }

    if (!is_any_errors) fprintf (ERROR_FILE, "Everything is correct!\n");
    PRINT_STARS_IN(ERROR_FILE, "*********");

    return OK;
}
