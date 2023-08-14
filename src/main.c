
#include <stdio.h>
#include <stdlib.h>
#include "centaur_rpc.h"
#include "centaur_pkg_list.h"
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc < 3) {
        fprintf(stderr, "error: too few arguments\n");
        return EXIT_FAILURE;
    } else if(argc > 3) {
        fprintf(stderr, "error: too many arguments\n");
        return EXIT_FAILURE;
    }
   
    centaur_pkg_list *l = NULL;

    if(strcmp(argv[1], "search") == 0) {
       l = centaur_rpc_search(argv[2]);
    } else if(strcmp(argv[1], "install") == 0) {
        //install a package
    }

    centaur_pkg_list_free(l);

    return EXIT_SUCCESS;
}
