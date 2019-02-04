#ifndef PARSEINI_H
#include "parseini.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

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
"OPTIONS:\n"
"  -f, --file <FILE>            path to file to read from\n"
"  -k, --key <KEY>              prints value of the matching key-value"
"pair\n"
"  -s, --section-only <SECTION> limits filters to the matching section\n"
"\nNote : If no FILE is passed,standard input is read";

static struct option long_options[] =
{
    {"file", required_argument, 0, 'f'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {"validate", no_argument, 0, 'V'},
    {"key", required_argument, 0, 'k'},
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

static size_t nstrlen (char *buf)
{
    char *i = buf;
    size_t siz = 0;
    while ( *i != '\n' && *i != COMMENT_DELIM)
    {
        siz++;
        i++;
    }
    return siz;
}


static size_t nstrcpy (char *dest, char *src)
{
    char *i = src,*j = dest;
    size_t bytes_copied = 0;
    while ( *i != '\n' && *i != COMMENT_DELIM )
    {
        *(j++) = *(i++);
        bytes_copied++;
    }
    *j = '\0';
    return bytes_copied;
}

static char* jump_to_newline (char *ptr)
{
    char *i = ptr;
    while ( (*i != '\0') && (*i != COMMENT_DELIM) )
    {
        if ( *i == '\n' )
            break;
        i++;
    }
    i++;
    return i;
}


static char* get_key_value (char *buf, char *key, int* found)
{
    char *val = NULL;
    char *i = buf,*j;
    size_t len = strlen (key), val_len;

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
            (void) nstrcpy (val, j);
            break;
        }
        i++;
    }
    return val;
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
    }
    else if ( i_opt->op & KEY )
    {
        while ( fread (buf, sizeof(char), BUFSIZ, f) )
        {
            strip_spaces (buf);
            val = get_key_value (buf, i_opt->key, &key_found);
            if ( key_found )
            {
                (void) fprintf (stdout, "Found %s%s\n", i_opt->key, val);
                free (val);
                break;
            }
        }
        if ( ! key_found )
            fprintf (stdout, "%s\n", KEY_MISMATCH);
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
    int opt, opt_index;
    i_opt = (optlist_t*) malloc (sizeof(optlist_t));
    e_assert (i_opt, E_MALLOC);

    i_opt->input_mode = UNINIT;
    i_opt->filepath = NULL;
    i_opt->key = NULL;
    i_opt->section = NULL;
    i_opt->op = UNINIT_OP;
    *err = E_SUCCESS;

    if ( argc < 2 )
        show_help (NOARGS_MSG);
    else
    {
        while (( opt = getopt_long (argc, argv, "+vchf:k:s:",
                        long_options, &opt_index)) != -1 )
        {
            if ( opt == 'h' )
            {
                show_help (FULL_MSG);
                break;
            }
            else if ( opt == 'v' )
            {
                show_help (BASIC_MSG);
                break;
            }
            else if ( opt == 'V' )
            {
                i_opt->op |= VALIDATE;
            }
            else if ( opt == 'f' )
            {
                i_opt->input_mode = FIL;
                i_opt->filepath = (char*) malloc (
                        (strlen (optarg)+1)*sizeof(char));
                e_assert (i_opt->filepath, E_MALLOC);
                (void) strcpy (i_opt->filepath, optarg);
            }
            else if ( opt == 'k' )
            {
                i_opt->op |= KEY;
                i_opt->key = (char*) malloc (
                        (strlen(optarg)+2) * sizeof(char));
                e_assert (i_opt->key, E_MALLOC);
                (void) strcpy (i_opt->key, optarg);
                (void) strcat (i_opt->key,equal_term);
            }
            else if ( opt == 's' )
            {
                i_opt->op |= SECTION;
                i_opt->section = (char*) malloc (
                        (strlen(optarg)+1) * sizeof(char));
                e_assert (i_opt->section, E_MALLOC);
                (void) strcpy (i_opt->section, optarg);
            }
            else if ( opt == '?' )
            {
                *err = E_ARG;
                break;
            }
            else
                *err = E_ARG;
        }
        if ( i_opt->input_mode == UNINIT )
            i_opt->input_mode = STDIN;
        if ( optind < argc )
            *err = E_ARG;
    }
    return i_opt;
}


