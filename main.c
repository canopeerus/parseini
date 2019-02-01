#include "parseini.h"
#include <stdio.h>

int main (int argc, char *argv[])
{
    error_t err;
    ini_opt_list *ini_opt = NULL;

    ini_opt_init (ini_opt);
    err = read_option (argc, argv, ini_opt);
    if ( err )
        (void) fprintf (stderr, "Unsupported option\n");

    ini_opt_cleanup (ini_opt);
    return 0;
}
