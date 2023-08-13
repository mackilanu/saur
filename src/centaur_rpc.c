#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <cjson/cJSON.h>
#include <stdbool.h>

#include "centaur_rpc.h"
#include "centaur_pkg_list.h"

struct memory {
    char *response;
    size_t size;
};

/**
 * This function runs after a call to the aur api has been made!
 */
size_t write_data(void *buffer, size_t size, size_t nmemb, void *clientp) {
    size_t realsize = size * nmemb;

    struct memory *mem = (struct memory *)clientp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), buffer, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    if(ptr == NULL)
        return 0;  /* out of memory! */

    return realsize;
}


int saur_parse_json(char *data) {
    int status = 0;
    cJSON *json = cJSON_Parse(data);
    const cJSON *results  = NULL;
    const cJSON *result = NULL;


    if(json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON parsing error before: %s\n", error_ptr);
        }

    } else {
        results = cJSON_GetObjectItemCaseSensitive(json, "results");
        int row = 1;

        centaur_pkg_list *l = centaur_pkg_list_init(1);
        cJSON_ArrayForEach(result, results) {
            cJSON *name = cJSON_GetObjectItemCaseSensitive(result, "Name");
            cJSON *popularity = cJSON_GetObjectItemCaseSensitive(result, "Popularity");
            cJSON *desc = cJSON_GetObjectItemCaseSensitive(result, "Description");

            centaur_pkg_list_item *item = malloc(sizeof(*item));

            item->name = name->valuestring;
            item->popularity = popularity->valuedouble;
            item->description = desc->valuestring;

            centaur_pkg_list_insert(l, item);
            //printf("%d) %s (%2f)\n\t%s\n",row, name->valuestring, popularity->valuedouble, desc->valuestring); /* Print a packag */
            row++;
        }



        for(int i = 0; i < centaur_pkg_list_size(l); i++) {
            printf("%s\n", centaur_pkg_list_item_at(l, i)->name);
        }
        centaur_pkg_list_free(l);
    }

    cJSON_Delete(json);

    return status;

}

void centaur_get_json_response(char *url) {
    CURL *curl = curl_easy_init();
    struct memory chunk = {0};

    if(curl) {
        char test[1024] = SAUR_AUR_BASE_URL;
        strcat(test, url);

        curl_easy_setopt(curl, CURLOPT_URL, test);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION , write_data);

        curl_easy_perform(curl);
        
        int stats = saur_parse_json(chunk.response);


        curl_easy_cleanup(curl);

        free(chunk.response);
    }
}

