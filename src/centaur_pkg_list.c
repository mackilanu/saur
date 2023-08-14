#include "centaur_pkg_list.h"
#include <stdio.h>
#include <stdlib.h>

struct centaur_pkg_list {
    centaur_pkg_list_item **list;
    int size;
};

centaur_pkg_list *centaur_pkg_list_init(size_t init_size) {
    centaur_pkg_list *l = malloc(sizeof(*l));
    l->list = calloc(1, sizeof(centaur_pkg_list_item));
    l->size = 0;
    return l;
}

centaur_pkg_list *centaur_pkg_list_insert(centaur_pkg_list *l, centaur_pkg_list_item *item) {
    l->list = realloc(l->list, (l->size + 1)*sizeof(*item) + 1);
    l->list[l->size] = item;
    l->size++;
    return l;
}

centaur_pkg_list_item *centaur_pkg_list_item_at(centaur_pkg_list *l, int index) {
    if(index > l->size)
        return NULL;

    return l->list[index];
}


void centaur_pkg_print(centaur_pkg_list *l) {
    for(int i = 0; i < centaur_pkg_list_size(l); i++) {
        printf("%d - %s (votes: %d) (popularity: +%.2f)\n\t%s\n", i+1,
               centaur_pkg_list_item_at(l, i)->name, 
               centaur_pkg_list_item_at(l, i)->votes,
               centaur_pkg_list_item_at(l, i)->popularity,
               centaur_pkg_list_item_at(l, i)->description
        );
    }
}

void centaur_pkg_list_free(centaur_pkg_list *l) {
    for(int i = 0; i < l->size; i++) {
        if(centaur_pkg_list_item_at(l, i)->name != NULL) {
            free(centaur_pkg_list_item_at(l, i)->name);
        } if(centaur_pkg_list_item_at(l, i)->description != NULL) {
            free(centaur_pkg_list_item_at(l, i)->description);
        } if(centaur_pkg_list_item_at(l, i)->urlpath != NULL) {
            free(centaur_pkg_list_item_at(l, i)->urlpath);
        }        

        free(centaur_pkg_list_item_at(l, i));
    }

    free(l->list);
    free(l);
}

int centaur_pkg_list_size(centaur_pkg_list *l) {
    return l->size;
}
