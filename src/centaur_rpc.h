#ifndef REQUEST_H

#define REQUEST_H

#define SAUR_AUR_BASE_URL "https://aur.archlinux.org/rpc/v5/search/"

#include <stdlib.h>
#include "centaur_pkg_list.h"

centaur_pkg_list *centaur_rpc_search(char *url);
#endif
