#ifndef PARSEINI_H
#define PARSEINI_H

#include <stdbool.h>
#include <stdint.h>

#define VERSION "v0.1.0"
#define E_MALLOC_MSG "Error : malloc failed"
#define E_ARG_MSG "Error : Unsupported option"
#define E_FILE_MSG "Error : unable to open file!"

typedef enum
{
    UNINIT_OP = 0,
    CHECK = 0x01,
    KEY = 0x02,
    SECTION = 0x04
} operation_t;

typedef enum
{
    NOARGS_MSG,
    BASIC_MSG,
    FULL_MSG
} msg_t;

typedef enum
{
    UNINIT,
    STDIN,
    FIL
} input_t;

typedef enum
{
    E_ARG = -1,
    E_SUCCESS,
    E_MALLOC,
    E_FILE
} error_t;

typedef struct
{
    input_t input_mode;
    int op;
    char *filepath;
    char *key;
    char *section;
} optlist_t;

extern optlist_t* read_option (int, char**, error_t*);

extern void serror (error_t);

extern void e_assert (bool, error_t);

extern void parseini (optlist_t*);
#endif
