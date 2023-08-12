#ifndef REQUEST_H

#define REQUEST_H

#define SAUR_AUR_BASE_URL "https://aur.archlinux.org/rpc/?v=5&type=search&arg="

#include <stdlib.h>

void saur_get_json_response(char *url);

#endif
