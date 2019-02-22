#ifndef PARSEINI_H
#define PARSEINI_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>


#define ULONG_MAX 0xFFFFFFFFUL
#define VERSION "v0.1.0"
#define E_MALLOC_MSG "Error: malloc failed"
#define E_ARG_MSG "Error: Argument error"
#define E_FILE_MSG "Error: unable to open file!"
#define E_HT_MSG "Error: hashtable error!"
#define KEY_MISMATCH "pi: No matching key-value pair found for option:"
#define COMMENT_DELIM ';'

typedef enum
{
    HELP_OP = -1,
    UNINIT_OP = 0,
    VALIDATE = 0x01,
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
    E_FILE,
    E_HT
} error_t;

typedef struct
{
    input_t input_mode;
    int op;
    char *filepath;
    char *key;
    size_t key_len;
    char *section;
    size_t section_len;
} optlist_t;

typedef struct entry
{
    char *key;
    char *value;
    struct entry *next;
}entry_t;

typedef struct
{
    size_t size;
    entry_t **table;
} hashtable_t;

extern optlist_t* read_option (int, char**, error_t*);

extern void serror (error_t);

extern void e_assert (bool, error_t);

extern void parseini (optlist_t*);
#endif
