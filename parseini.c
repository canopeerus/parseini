#ifndef PARSEINI_H
#include "parseini.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

static const char *noargs_msg = "No options given. Run with '-h' option";
static const char *basic_msg = "parseini v0.1.0\n"
"canopeerus <visvanathannaditya@gmail.com\n"
"A command-line simple INI configuration processor";
static const char *full_msg = "Full msg";

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
            (void) fprintf (stdout, "%s\n", full_msg);
            break;
    }
    return;
}

void ini_opt_init (ini_opt_list* i_opt)
{
    i_opt->input_mode = UNINIT;
    i_opt->filepath = NULL;
}
error_t read_option (int argc, char *argv[], ini_opt_list* ini_opt)
{
    ini_opt = (ini_opt_list*) malloc (sizeof(ini_opt_list));

    ini_opt_init (ini_opt);

    int opt, err = 0;
    if ( argc < 2 )
        show_help (NOARGS_MSG);
    else
    {
        while (( opt = getopt (argc, argv, "is+vVhf:")) != -1 )
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
                {
                    ini_opt->input_mode = STDIN;
                }
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
