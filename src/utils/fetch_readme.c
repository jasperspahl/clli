//
// Created by jasper on 02.02.23.
//

#include "fetch_readme.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

typedef struct {
	char *response;
	size_t size;
} memory;

void init_memory(memory *mem) {
	mem->response = malloc(1);
	mem->size = 0;
}

// Callback function for curl_easy_setopt
// https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
size_t write_data(void *ptr, size_t size, size_t nmemb, void *userp) {
	size_t real_size = size * nmemb;
	memory *mem = (memory *) userp;
	char *p = realloc(mem->response, mem->size + real_size + 1);
	if (mem->response == NULL) {
		return 0;
	}
	mem->response = p;
	memcpy(&(mem->response[mem->size]), ptr, real_size);
	mem->size += real_size;
	mem->response[mem->size] = 0;
	return real_size;
}

char *fetch_readme(char *repo) {
	if (repo == NULL) {
		return NULL;
	}
	CURL *curl = curl_easy_init();
	if (curl == NULL) {
		return NULL;
	}

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/vnd.github.raw");

	char *url = malloc(strlen(repo) + 36);
	strcpy(url, "https://api.github.com/repos/");
	strcat(url, repo);
	strcat(url, "/readme");

	memory *readme = malloc(sizeof(memory));
	init_memory(readme);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, readme);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "clli");

	CURLcode res = curl_easy_perform(curl);

	long http_code = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	if (res != CURLE_OK || http_code != 200) {
		free(readme->response);
		free(readme);
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
		free(url);
		return NULL;
	}
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	free(url);
	return readme->response;
}