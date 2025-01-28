#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <limits.h>

const int POISON = 999;

const char* const dump_file = "dump_file.txt";
const char* const error_file = "error_file.txt";

typedef int stackElem;

/*struct stack_inf
{
    const int LEFT_СANARY = POISON;
    int line = __LINE__;
    int file = __FILE__;
    int func = __func__;
    const int RIGHT_СANARY = POISON;
};*/

struct stack_t
{
    const int LEFT_СANARY = POISON;
    stackElem* data = nullptr;
    size_t size = INT_MAX;
    size_t capacity = INT_MAX;
    const int RIGHT_СANARY = POISON;
};

enum stackErrors
{
    OK =                 0,
    STK_NULL =           1,
    STK_CANARY_ERR =     2,
    DATA_EMPTY =         3,
    SIZE_ERR =           4,
    CAPACITY_ERR =       5,
    SIZE_OVERFLOW =      6,
    ELEMENT_ERR =        7,
    ERR =                8,
    REALLOC_ERR =        9,
    CTOR_ERR =          10,
    CANARY_ERR =        11,
    DUMP_ERR =          12,
    USER_ERR =          13,
    DUMPED =            14,
    FILE_ERR =          15
};

stackErrors stackCtor (stack_t* stk, size_t capacity);

stackErrors stackDtor (stack_t* stk);

stackErrors stackDump (stack_t* stk, const char* func);

stackErrors stackPush (stack_t* stk, stackElem value);

stackErrors stackPop (stack_t* stk, stackElem* value);

stackErrors stackVerify (stack_t* stk, const char* file, size_t line, const char* func);

#define SPEC "%d"

#define PRINT_STARS_IN(filename, func); \
        fprintf (filename, "\n************************%s****************************\n\n", func); //TODO - как-то скорректировать

#define PRINT_ERR \
        fprintf (ERROR_FILE, "\nERROR in file - %s, in line - %zu, in function - %s:\n", file, line, func);

#define STACK_CHECK(stk);                                          \
        if (stackVerify (stk, __FILE__, __LINE__, __func__) != OK) \
        fprintf (stderr, "ERROR_FILE doesn't open\n");               \
        if (stackDump (stk, __func__) != OK)                       \
        fprintf (stderr, "DUMP_FILE doesn't open\n");

#endif // STACK_H
