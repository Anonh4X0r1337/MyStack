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
    ERR =                1,
    DATA_EMPTY =         2,
    SIZE_OVERFLOW =      3,
    STK_NULL =           4,
    REALLOC_ERR =        5,
    CTOR_ERR =           6,
    CANARY_ERR =         7,
    DUMP_ERR =           8,
    USER_ERR =           9,
    SIZE_ERR =          10,
    CAPACITY_ERR =      11,
    DUMPED =            12,
    STK_CANARY_ERR =    13,
    ELEMENT_ERR =       14,
    FILE_ERR =          15
};

stackErrors stackCtor (stack_t* stk, size_t capacity);

stackErrors stackDtor (stack_t* stk);

stackErrors stackDump (stack_t* stk, const char* func);

stackErrors stackPush (stack_t* stk, stackElem value);

stackErrors stackPop (stack_t* stk, stackElem* value);

stackErrors stackVerify (stack_t* stk, const char* file, size_t line, const char* func);

#define SPEC "%d"

#define PRINT_STARS_IN(filename, calling_file); \
        fprintf (filename, "\n************************%s****************************\n\n", func); //TODO - как-то скорректировать

#define PRINT_ERR \
        fprintf (ERROR_FILE, "\nERROR in file - %s, in line - %zu, in function - %s\n", file, line, func);

#define STACK_CHECK(stk);                                \
        stackVerify (stk, __FILE__, __LINE__, __func__); \
        stackDump (stk, __func__);

#endif // STACK_H
