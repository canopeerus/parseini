#ifndef PARSEINI_H
#include "parseini.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

static const char *noargs_msg = "No options given. Run with '-h' option";

static const char *basic_msg =
"parseini v0.1.0\n"
"canopeerus <visvanathannaditya@gmail.com\n"
"A command-line simple INI configuration processor";

static const char *full_msg =
"\nUSAGE:\n"
"\tpi [FLAGS] [OPTIONS] <path/to/file>\n"
"\nFLAGS:\n"
"\t-h   prints help information\n"
"\t-v   prints version information\n"
"\t-V   checks validity of input INI\n"
"\t-s   read INI input from stdin (conflicts with '-f <file' option)\n"
"\nOPTIONS:\n"
"\t-f <FILE>    path to file to read from";

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
            (void) fprintf (stdout,"%s\n", basic_msg);
            (void) fprintf (stdout, "%s\n", full_msg);
            break;
    }
    return;
}

void ini_opt_init (ini_opt_list* i_opt)
{
    i_opt = (ini_opt_list*) malloc (sizeof(ini_opt_list));
    i_opt->input_mode = UNINIT;
    i_opt->filepath = NULL;
}

void ini_opt_cleanup (ini_opt_list* i_opt)
{
    free (i_opt);
}

error_t read_option (int argc, char *argv[], ini_opt_list* ini_opt)
{
    int opt;
    error_t err = E_SUCCESS;
    if ( argc < 2 )
        show_help (NOARGS_MSG);
    else
    {
        while (( opt = getopt (argc, argv, "+isvVhf:")) != -1 )
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
                (void) fprintf (stdout, "Validate option chosen\n");

            else if ( opt == 'f' )
            {
                if ( ini_opt->input_mode == STDIN )
                {
                    (void) fprintf (stderr, "Conflicting arguments"
                            " 'file' and 'stdin'\n");
                    err = E_ARG;
                    break;
                }
                else
                {
                    ini_opt->filepath = (const char*) optarg;
                    ini_opt->input_mode = FIL;
                }
            }

            else if ( opt == 's' )
            {
                if ( ini_opt->input_mode == FIL || ini_opt->filepath )
                {
                    (void) fprintf (stderr, "Conflicting arguments"
                            " 'file' and 'stdin'\n");
                    err = E_ARG;
                    break;
                }
                else
                    ini_opt->input_mode = STDIN;
            }


            else if ( opt == '?' )
            {
                err = 1;
                break;
            }
            else
                err = 1;
        }
        if ( optind < argc )
            err = 1;
    }
    return err;
}
