#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

#include "internal/common.h"
#include "internal/json.h"
#include "internal/parse.h"
#include "json_object.h"
#include "methods.h"

#define opt_size(arr) (sizeof(arr) / sizeof(arr[0]))

static size_t write_callback(const void *ptr, size_t size, size_t nmemb, char *userdata) {
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

static size_t discard_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	(void)userdata;
	(void)ptr;
	return size * nmemb;
}

static int tgbot_request(const char *url, struct memory_buffer **mb, json_object *json) {
	CURL *curl = curl_easy_init();
	if (!curl) {
		return -1;
	}

	const char *json_string = NULL;

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	if (mb != NULL) {
		*mb = calloc(1, sizeof(struct memory_buffer));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, (curl_write_callback)write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, *mb);
	} else {
		struct memory_buffer *mb_f = malloc(sizeof *mb_f);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, mb_f);
		if (mb_f) {
			free(mb_f);
		}
	}

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 30L);

	if (json != NULL) {
		json_string = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PLAIN);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string);
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

static int tgbot_execute_method(const tgbot_s *bot, const char *method, tgbot_option_s *options, size_t optlen) {
	char url[URL_LEN] = {0};
	snprintf(url, sizeof(url), "%s%s", bot->api, method);

	json_object *rjson = json_builder(options, optlen);
	int ret = tgbot_request(url, NULL, rjson);
	json_object_put(rjson);

	return ret;
}

int tgbot_get_update(tgbot_s *bot, tgbot_update_s *update, Callback cbq_handler) {
	char url[URL_LEN];

	memset(update, 0, sizeof(tgbot_update_s));

	int limit = 1;
	int timeout = 30;
	tgbot_option_s options[3] = {
		{"offset", &bot->offset, tgbot_opt_int64},
		{"limit", &limit, tgbot_opt_int},
		{"timeout", &timeout, tgbot_opt_int},
	};
	json_object *rjson = json_builder(options, 3);

	snprintf(url, sizeof(url), "%sgetUpdates", bot->api);

	struct memory_buffer *mb = {0};
	int ret = tgbot_request(url, &mb, rjson);
	json_object_put(rjson);
	if (ret != 0) {
		if (mb) {
			free(mb->data);
			free(mb);
		}

		return -1;
	}

	json_object *json = json_tokener_parse(mb->data);
	free(mb->data);
	free(mb);

	const json_object *ok = json_object_object_get(json, "ok");
	if (!json_object_is_type(ok, json_type_boolean) || !json_object_get_boolean(ok)) {
		json_object_put(json);

		return -1;
	}

	const json_object *results = json_object_object_get(json, "result");
	size_t results_len = json_object_array_length(results);

	if (results_len == 0) {
		json_object_put(json);

		return 0;
	}

	/* Check if it is a Message or a CallbackQuery*/
	json_object *result = json_object_array_get_idx(results, 0);
	const json_object *message = json_object_object_get(result, "message");
	if (message) {
		tgbot_parse_message(bot, update, result);
	} else if (cbq_handler != NULL) {
		tgbot_cbquery_s query;
		tgbot_parse_cbquery(bot, &query, result, cbq_handler);
	}

	json_object_put(json);

	return 0;
}

int tgbot_get_me(const tgbot_s *bot, tgbot_me_s *me) {
	char url[URL_LEN];
	snprintf(url, sizeof(url), "%sgetMe", bot->api);

	struct memory_buffer *mb = {0};
	int ret = tgbot_request(url, &mb, NULL);
	if (ret != 0) {
		if (mb) {
			free(mb->data);
			free(mb);
		}

		return -1;
	}

	json_object *json = json_tokener_parse(mb->data);
	free(mb->data);
	free(mb);

	const json_object *ok = json_object_object_get(json, "ok");
	if (!json_object_get_boolean(ok)) {
		json_object_put(json);

		return -1;
	}

	const json_object *result = json_object_object_get(json, "result");
	json_object *first_name = json_object_object_get(result, "first_name");
	snprintf(me->first_name, sizeof(me->first_name), "%s", json_object_get_string(first_name));

	json_object *username = json_object_object_get(result, "username");
	snprintf(me->username, sizeof(me->username), "%s", json_object_get_string(username));

	json_object_put(json);

	return 0;
}

int tgbot_send_message(const tgbot_s *bot, int64_t chat_id, const char *text, const char *parse_mode,
					   tgbot_inlinekeyboard_s *reply_markup) {
	tgbot_option_s options[4] = {
		{"chat_id", &chat_id, tgbot_opt_int64},
		{"text", (void *)text, tgbot_opt_string},
		{"parse_mode", (void *)parse_mode, tgbot_opt_string},
		{"reply_markup", reply_markup, tgbot_opt_inlinekeyboard},
	};

	return tgbot_execute_method(bot, "sendMessage", options, opt_size(options));
}

int tgbot_edit_message_text(const tgbot_s *bot, int64_t chat_id, long message_id, const char *text,
							tgbot_inlinekeyboard_s *keyboard) {
	tgbot_option_s options[4] = {
		{"chat_id", &chat_id, tgbot_opt_int64},
		{"message_id", &message_id, tgbot_opt_int},
		{"text", (void *)text, tgbot_opt_string},
		{"reply_markup", keyboard, tgbot_opt_inlinekeyboard},
	};

	return tgbot_execute_method(bot, "editMessageText", options, opt_size(options));
}

int tgbot_send_dice(const tgbot_s *bot, int64_t chat_id, const char *emoji) {
	tgbot_option_s options[2] = {
		{"chat_id", &chat_id, tgbot_opt_int64},
		{"emoji", (void *)emoji, tgbot_opt_string},
	};

	return tgbot_execute_method(bot, "sendDice", options, opt_size(options));
}
