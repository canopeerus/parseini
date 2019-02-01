#include "parseini.h"
#include <stdio.h>

int main (int argc, char *argv[])
{
    int err;
    ini_opt_list *ini_opt = NULL;
    err = read_option (argc, argv, ini_opt);
    if ( err )
        (void) fprintf (stderr, "Unsupported option\n");
    return 0;
}
