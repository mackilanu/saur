
#include <stdio.h>
#include <stdlib.h>
#include "request.h"
#include <string.h>

int main(int argc, char *argv[])
{
    saur_get_json_response("linux");
    printf("====================================================\n");
    saur_get_json_response("jetbrains");
    return EXIT_SUCCESS;
}
