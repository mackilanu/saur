#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <cjson/cJSON.h>

#include "request.h"

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

void saur_parse_json(char *data) {
    cJSON *json = cJSON_Parse(data);
    char *string = cJSON_Print(json);
    printf("%s", string);
    free(string);
    cJSON_Delete(json);
}

void saur_get_json_response(char *url) {
    CURL *curl = curl_easy_init();
    struct memory chunk = {0};

    if(curl) {
        char test[4096] = SAUR_AUR_BASE_URL;
        strcat(test, url);

        curl_easy_setopt(curl, CURLOPT_URL, test);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION , write_data);

        curl_easy_perform(curl);
        
        saur_parse_json(chunk.response);
        //printf("%d", res);
        curl_easy_cleanup(curl);

        free(chunk.response);
    }
}

