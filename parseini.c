#ifndef PARSEINI_H
#include "parseini.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define _XOPEN_SOURCE 500

static const char *noargs_msg = "No options given. Run 'pi -h' for help";
static const char equal_term[2] = {'=','\0'};
static const char *basic_msg =
"parseini "VERSION"\n"
"canopeerus <visvanathannaditya@gmail.com>\n"
"A simple command-line INI configuration processor";

static const char *full_msg =
"\nUSAGE:\n"
"  pi [FLAGS] [OPTIONS]\n"
"\nFLAGS:\n"
"  -h, --help                   prints help information\n"
"  -v, --version                prints version information\n"
"  -V, --validate               checks validity of input INI\n"
"\nOPTIONS:\n"
"  -F, --file <FILE>            path to file to read from\n"
"  -m, --match-option <KEY>     prints value of the matching key-value"
" pair\n"
"  -s, --section-only <SECTION> limits filters to the matching section\n"
"\nNote : If no FILE is passed,standard input is read";

static struct option long_options[] =
{
    {"file", required_argument, 0, 'F'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {"validate", no_argument, 0, 'V'},
    {"match-option", required_argument, 0, 'm'},
    {"section-only", required_argument, 0, 's'},
    {0, 0, 0, 0}
};

static void show_help (msg_t m)
{
    switch (m)
    {
        case NOARGS_MSG:
            (void) fprintf (stderr, "%s\n", noargs_msg);
            break;
        case BASIC_MSG:
            (void) fprintf (stdout, "%s\n", basic_msg);
            break;
        case FULL_MSG:
            (void) fprintf (stdout, "%s\n", basic_msg);
            (void) fprintf (stdout, "%s\n", full_msg);
            break;
    }
}

static void strip_spaces (char* buf)
{
    char *i = buf,*j = buf;
    while ( *j != '\0' )
    {
        *i = *j++;
        if ( *i != ' ' )
            i++;
    }
    *i = '\0';
}


static int validate_ini (char* buf)
{
    // do nothing
    return 0;
}

// Computes length of string starting at buf until '\n' or COMMENT_DELIM
static size_t nstrlen (char *buf)
{
    char *i = buf;
    size_t siz = 0;
    while ( *i != NEWLINE && *i != COMMENT_DELIM)
    {
        siz++;
        i++;
    }
    return siz;
}


// Similar to strcpy but copies strings till newline '\n' or COMMENT_DELIM
static size_t nstrcpy (char *dest, char *src)
{
    char *i = src,*j = dest;
    size_t bytes_copied = 0;
    while ( *i != NEWLINE && *i != COMMENT_DELIM )
    {
        *(j++) = *(i++);
        bytes_copied++;
    }
    *j = '\0';
    return bytes_copied;
}

// jump to the next non-comment line
static char* jump_to_newline (char *ptr)
{
    char *i = ptr;
    while ( (*i != '\0') && (*i != COMMENT_DELIM) )
    {
        if ( *i == NEWLINE )
            break;
        i++;
    }
    return ++i;
}


static char* get_key_value (char *buf, char *key, int* found)
{
    char *val = NULL;
    char *i = buf,*j;
    size_t len = strlen (key), val_len, ret;

    *found = 0;
    while ( *i != '\0' )
    {
        if ( *i == COMMENT_DELIM )
            i = jump_to_newline (i+1);
        if ( strncmp (i, key, len) == 0 )
        {
            *found = 1;
            j = i + len;
            val_len = nstrlen (j);
            val = (char*) malloc ( (val_len+1) * sizeof(char));
            e_assert (val, E_MALLOC);
            ret = nstrcpy (val, j);
            assert (ret > 0);
            break;
        }
        i++;
    }
    *(key + len - 1) = 0;
    return val;
}

// constructor of sorts for optlist_t
static void optlist_t_init (optlist_t* opt)
{
    opt->input_mode = UNINIT;
    opt->filepath = NULL;
    opt->key = NULL;
    opt->section = NULL;
    opt->op = UNINIT_OP;

}


static void read_file_chunk (FILE* f, optlist_t* i_opt)
{
    int v_err,key_found;
    char *buf = NULL;
    char *val = NULL;
    buf = (char*) malloc (BUFSIZ * sizeof(char));
    e_assert (buf, E_MALLOC);

    if ( i_opt->op & VALIDATE )
    {
        // while ( fread (buf, sizeof(char), BUFSIZ, f) );
    }
    else if ( i_opt->op & KEY )
    {
        while ( fread (buf, sizeof(char), BUFSIZ, f) )
        {
            strip_spaces (buf);
            val = get_key_value (buf, i_opt->key, &key_found);
            if ( key_found )
            {
                (void) fprintf (stdout, "%s\n", val);
                free (val);
                break;
            }
        }
        if ( ! key_found )
            (void) fprintf (stdout, "%s %s\n", KEY_MISMATCH, i_opt->key);
    }
    free (buf);
}

void serror (error_t e)
{
    switch (e)
    {
        case E_MALLOC:
            (void) fprintf (stderr, "%s\n", E_MALLOC_MSG);
            break;
        case E_ARG:
            (void) fprintf (stderr, "%s\n", E_ARG_MSG);
            break;
        case E_FILE:
            (void) fprintf (stderr, "%s\n", E_FILE_MSG);
            break;
        case E_HT:
            (void) fprintf (stderr, "%s\n", E_HT_MSG);
            break;
        case E_SUCCESS:
            // do nothing
            break;
    }
}

void e_assert (bool expr,error_t err)
{
    if ( ! expr )
    {
        serror (err);
        exit((int)err);
    }
}

void parseini (optlist_t* i_opt)
{
    FILE* f = NULL;
    if ( i_opt->op )
    {
        if ( i_opt->input_mode == FIL )
        {
            f = fopen (i_opt->filepath, "r");
            e_assert (f, E_FILE);
            read_file_chunk (f, i_opt);
            fclose (f);
        }
        else if ( i_opt->input_mode == STDIN )
        {
            read_file_chunk (stdin, i_opt);
        }
    }
}

optlist_t* read_option (int argc, char *argv[], error_t *err)
{
    optlist_t* i_opt = NULL;
    int opt;
    i_opt = (optlist_t*) malloc (sizeof(optlist_t));
    e_assert (i_opt, E_MALLOC);

    optlist_t_init (i_opt);

    if ( argc < 2 )
        show_help (NOARGS_MSG);
    else
    {
        while (( opt = getopt_long (argc, argv, "vVhF:m:s:",
                        long_options, NULL)) != -1 )
        {
            if ( opt == 'h' )
            {
                show_help (FULL_MSG);
                i_opt->op = HELP_OP;
                break;
            }
            else if ( opt == 'v' )
            {
                show_help (BASIC_MSG);
                i_opt->op = HELP_OP;
                break;
            }
            else if ( opt == 'V' )
            {
                i_opt->op |= VALIDATE;
            }
            else if ( opt == 'F' )
            {
                i_opt->input_mode = FIL;
                i_opt->filepath = (char*) malloc (
                        (strlen (optarg)+1)*sizeof(char));
                e_assert (i_opt->filepath, E_MALLOC);
                (void) strcpy (i_opt->filepath, optarg);
            }
            else if ( opt == 'm' )
            {
                i_opt->op |= KEY;
                i_opt->key_len = strlen (optarg);
                i_opt->key = (char*) malloc (
                        (i_opt->key_len + 2) * sizeof(char));
                e_assert (i_opt->key, E_MALLOC);
                strcpy (i_opt->key, optarg);
                (void) strcat (i_opt->key,equal_term);

            }
            else if ( opt == 's' )
            {
                i_opt->op |= SECTION;
                i_opt->section_len= strlen (optarg);
                i_opt->section = (char*) malloc (
                        (i_opt->section_len + 3) * sizeof(char));
                e_assert (i_opt->section, E_MALLOC);
                strcpy (i_opt->section, optarg);
            }
            else if ( opt == '?' )
            {
                *err = E_ARG;
                break;
            }
            else
            {
                (void) fprintf (stdout, "%c\n", opt);
                *err = E_ARG;
                break;
            }
        }
        if ( i_opt->input_mode == UNINIT && i_opt->op != HELP_OP )
            i_opt->input_mode = STDIN;
        if ( optind < argc )
            *err = E_ARG;
        if ( i_opt->op == UNINIT_OP && *err != E_ARG )
        {
            (void) fprintf (stderr, "pi: No operations specified\n");
        }
    }
    return i_opt;
}
