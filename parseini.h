#ifndef PARSEINI_H
#define PARSEINI_H

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
    const char *filepath;
} ini_opt_list;

extern error_t read_option (int, char**, ini_opt_list*);

#endif
