#ifndef PARSEINI_H
#include "parseini.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

static const char *noargs_msg = "No options given. Run with '-h' option";

static const char *basic_msg =
"parseini "VERSION"\n"
"canopeerus <visvanathannaditya@gmail.com>\n"
"A simple command-line INI configuration processor";

static const char *full_msg =
"\nUSAGE:\n"
"\tpi [FLAGS] [OPTIONS] <path/to/file>\n"
"\nFLAGS:\n"
"\t-h, --help           prints help information\n"
"\t-v, --version        prints version information\n"
"\t-c, --check          checks validity of input INI\n"
"\nOPTIONS:\n"
"\t-f, --file <FILE>    path to file to read from\n"
"\nNote : If no FILE is passed,standard input is read";

static struct option long_options[] =
{
    {"file", required_argument, 0, 'f'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {"check", no_argument, 0, 'c'},
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

optlist_t* read_option (int argc, char *argv[], error_t *err)
{
    optlist_t* i_opt = NULL;
    int opt, opt_index;
    i_opt = (optlist_t*) malloc (sizeof(optlist_t));
    e_assert (i_opt, E_MALLOC);

    i_opt->input_mode = UNINIT;
    i_opt->filepath = NULL;
    *err = E_SUCCESS;

    if ( argc < 2 )
        show_help (NOARGS_MSG);
    else
    {
        while (( opt = getopt_long (argc, argv, "+vchf:", long_options,
                        &opt_index)) != -1 )
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
            else if ( opt == 'c' )
                (void) fprintf (stdout, "Validate option chosen\n");
            else if ( opt == 'f' )
            {
                i_opt->input_mode = FIL;
                i_opt->filepath = (char*) malloc (
                        (strlen (optarg)+1)*sizeof(char));
                e_assert (i_opt->filepath, E_MALLOC);
                (void) strcpy (i_opt->filepath, optarg);

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
