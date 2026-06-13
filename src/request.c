#include "internal/request.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
	size_t real_size = size * nmemb;
	struct memory_buffer *mem = (struct memory_buffer *)userdata;

	char *tmp = realloc(mem->data, mem->size + real_size + 1);
	if (!tmp) {
		return 0;
	}
	mem->data = tmp;

	memcpy(&(mem->data[mem->size]), ptr, real_size);
	mem->size += real_size;
	mem->data[mem->size] = '\0';

	return real_size;
}

static size_t discard_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
	(void)ptr;
	(void)userdata;
	return size * nmemb;
}

int tgbot_request(const char *url, struct memory_buffer **mb, const char *json) {
	CURL *curl = curl_easy_init();
	if (!curl) {
		return -1;
	}

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

	if (mb != NULL) {
		*mb = calloc(1, sizeof(struct memory_buffer));
		if (!*mb) {
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
			return -1;
		}
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, *mb);
	} else {
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_cb);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
	}

	if (json != NULL) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
	}

	CURLcode res = curl_easy_perform(curl);
	curl_slist_free_all(headers);

	if (res != CURLE_OK) {
		curl_easy_cleanup(curl);
		if (mb != NULL && *mb) {
			free((*mb)->data);
			free(*mb);
			*mb = NULL;
		}
		return -1;
	}

	curl_easy_cleanup(curl);
	return 0;
}
