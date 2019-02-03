#include "parseini.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    error_t err;
    optlist_t *ini_opt = NULL;

    ini_opt = read_option (argc, argv, &err);
    if ( err == E_ARG )
    {
        serror (E_ARG);
        exit ((int) E_ARG);
    }
    else
    {
        parseini (ini_opt);
    }
    return 0;
}
