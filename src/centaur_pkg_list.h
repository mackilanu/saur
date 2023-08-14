#ifndef CENTAUR_PKG_LIST_H
#define CENTAUR_PKG_LIST_H

#include <stdlib.h>


typedef struct centaur_pkg_list centaur_pkg_list;
typedef struct centaur_pkg_list_item centaur_pkg_list_item;

struct centaur_pkg_list_item {
    char *name;
    char *description;
    double popularity;
    int votes;
    char *urlpath;
};


centaur_pkg_list *centaur_pkg_list_init(size_t init_size);
centaur_pkg_list *centaur_pkg_list_insert(centaur_pkg_list *l, centaur_pkg_list_item *item);
centaur_pkg_list_item *centaur_pkg_list_item_at(centaur_pkg_list *l, int index);
int centaur_pkg_list_size(centaur_pkg_list *l);
void centaur_pkg_list_free(centaur_pkg_list *l);

#endif
