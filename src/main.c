
#include <stdio.h>
#include <stdlib.h>
#include "centaur_rpc.h"
#include "centaur_pkg_list.h"
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>

static int verbose_flag;

int main(int argc, char *argv[])
{

    centaur_pkg_list *l = NULL;

    if ((argc-optind) > 2) {
        fprintf(stderr, "too many arguments\n");
        exit(EXIT_FAILURE);
    }

    if ((argc-optind) < 2) {
        fprintf(stderr, "missing argument\n");
        exit(EXIT_FAILURE);
    }

    char *searchval = NULL;
    char *installval = NULL;

    if(strcmp(argv[optind], "search") == 0) {
        printf("Performing search...\n");
        searchval = argv[optind+1];
    } else if(strcmp(argv[optind], "install") == 0) {
        printf("Performing install..\n.");
        installval = argv[optind+1];
    }else {
        fprintf(stderr, "invalid option '%s'\n", argv[optind]);
        exit(EXIT_FAILURE);
    }

    if(searchval != NULL) {
        l = centaur_rpc_search(searchval);
        centaur_pkg_print(l);
    } else if(installval != NULL) {
        l = centaur_rpc_search(installval);
    }

    centaur_pkg_list_free(l);
}
