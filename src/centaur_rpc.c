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



/**
 * Copy an entire string from src to dest.
 * */
void _centaur_cpy_string(char *dest, char *src) {

    for(; *src; src++, dest++) {
        *dest = *src;
    }

    *dest = '\0';
}


centaur_pkg_list *_centaur_parse_json(char *data) {
    int status = 0;
    cJSON *json = cJSON_Parse(data);
    const cJSON *results  = NULL;
    const cJSON *result = NULL;

    centaur_pkg_list *l = centaur_pkg_list_init(1);

    if(json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON parsing error before: %s\n", error_ptr);
        }

    } else {
        results = cJSON_GetObjectItemCaseSensitive(json, "results");
        int row = 1;

        cJSON_ArrayForEach(result, results) {
            cJSON *name = cJSON_GetObjectItemCaseSensitive(result, "Name");
            cJSON *popularity = cJSON_GetObjectItemCaseSensitive(result, "Popularity");
            cJSON *desc = cJSON_GetObjectItemCaseSensitive(result, "Description");
            cJSON *urlpath = cJSON_GetObjectItemCaseSensitive(result, "URLPath");
            cJSON *votes = cJSON_GetObjectItemCaseSensitive(result, "NumVotes");

            centaur_pkg_list_item *item = malloc(sizeof(*item));

            item->name = NULL;
            item->urlpath = NULL;
            item->description = NULL;

            if(name->valuestring != NULL) {
                item->name = malloc(strlen(name->valuestring) + 1);
                _centaur_cpy_string(item->name, name->valuestring);
            }

            if(desc->valuestring != NULL) {
                item->description = malloc(strlen(desc->valuestring) + 1);
                _centaur_cpy_string(item->description, desc->valuestring);
            }

            if(urlpath->valuestring != NULL) {
                item->urlpath = malloc(strlen(urlpath->valuestring) + 1);
                _centaur_cpy_string(item->urlpath, urlpath->valuestring);
            }

            item->popularity = popularity->valuedouble;
            item->votes = votes->valueint;

            centaur_pkg_list_insert(l, item);
            row++;
        }

        cJSON_Delete(json);
    }

    return l;
}

centaur_pkg_list *centaur_rpc_search(char *url) {
    CURL *curl = curl_easy_init();
    centaur_pkg_list *l; 
    struct memory chunk = {0};

    if(curl) {
        char test[1024] = SAUR_AUR_BASE_URL;
        strcat(test, url);

        curl_easy_setopt(curl, CURLOPT_URL, test);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION , write_data);

        curl_easy_perform(curl);
        
        l = _centaur_parse_json(chunk.response);
        curl_easy_cleanup(curl);

        free(chunk.response);
    }

    return l;
}

