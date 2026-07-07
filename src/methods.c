#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <yyjson.h>

#include "internal/json.h"
#include "internal/parser.h"
#include "internal/request.h"
#include "tgbot/methods.h"

#define opt_size(arr) (sizeof(arr) / sizeof(arr[0]))

static size_t discard_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
	(void)ptr;
	(void)userdata;
	return size * nmemb;
}

int execute_method(const tgbot_s *bot, const char *method, tgbot_option_s *options, size_t optlen) {
	char url[URL_LEN] = {0};
	snprintf(url, sizeof(url), "%s%s", bot->api, method);

	char *json = json_builder(options, optlen);
	int ret = tgbot_request(url, NULL, json);
	free(json);

	return ret;
}

static int execute_method_multipart(const tgbot_s *bot, const char *method, int64_t chat_id, const char *path,
									const char *caption) {
	CURL *curl = curl_easy_init();
	if (!curl) {
		return -1;
	}

	char url[URL_LEN] = {0};
	int chars = snprintf(url, sizeof(url), "%s%s", bot->api, method);
	if (chars < 0 || (size_t)chars >= sizeof(url)) {
		curl_easy_cleanup(curl);
		return -1;
	}

	char chat_id_str[32];
	snprintf(chat_id_str, sizeof(chat_id_str), "%ld", chat_id);

	curl_mime *mime = curl_mime_init(curl);
	curl_mimepart *part;

	part = curl_mime_addpart(mime);
	curl_mime_data(part, chat_id_str, CURL_ZERO_TERMINATED);
	curl_mime_name(part, "chat_id");

	part = curl_mime_addpart(mime);
	curl_mime_filedata(part, path);
	curl_mime_name(part, "photo");

	part = curl_mime_addpart(mime);
	curl_mime_data(part, caption, CURL_ZERO_TERMINATED);
	curl_mime_name(part, "caption");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

	CURLcode res = curl_easy_perform(curl);
	curl_mime_free(mime);
	curl_easy_cleanup(curl);

	if (res != CURLE_OK) {
		return -1;
	}

	return 0;
}

int tgbot_get_updates(tgbot_s *bot, tgbot_updates_t *updates) {
	if (!bot || !updates) {
		return -1;
	}

	updates->items = NULL;
	updates->count = 0;

	int limit = 100;
	int timeout = 30;
	tgbot_option_s options[3] = {
		{"offset", &bot->offset, tgbot_opt_int64},
		{"limit", &limit, tgbot_opt_int32},
		{"timeout", &timeout, tgbot_opt_int32},
	};
	char *json_str = json_builder(options, 3);

	char url[URL_LEN];
	snprintf(url, sizeof(url), "%sgetUpdates", bot->api);

	struct memory_buffer *mb = NULL;
	int ret = tgbot_request(url, &mb, json_str);
	free(json_str);
	if (ret != 0) {
		return -1;
	}

	yyjson_doc *doc = yyjson_read(mb->data, mb->size, 0);
	free(mb->data);
	free(mb);
	if (!doc) {
		return -1;
	}

	yyjson_val *root = yyjson_doc_get_root(doc);

	/* Check "ok" */
	yyjson_val *ok = yyjson_obj_get(root, "ok");
	if (!ok || !yyjson_is_bool(ok) || !yyjson_get_bool(ok)) {
		yyjson_doc_free(doc);
		return -1;
	}

	yyjson_val *results = yyjson_obj_get(root, "result");
	if (!results || !yyjson_is_arr(results)) {
		yyjson_doc_free(doc);
		return -1;
	}

	size_t count = yyjson_arr_size(results);
	if (count == 0) {
		yyjson_doc_free(doc);
		return 0;
	}

	updates->items = calloc(count, sizeof(tgbot_update_result_t));
	if (!updates->items) {
		yyjson_doc_free(doc);
		return -1;
	}

	size_t parsed = 0;
	int64_t max_offset = bot->offset;
	yyjson_val *val;
	yyjson_arr_iter iter;
	yyjson_arr_iter_init(results, &iter);

	while ((val = yyjson_arr_iter_next(&iter)) != NULL) {
		if (tgbot_parse_update(&updates->items[parsed], val) == 0) {
			int64_t uid = updates->items[parsed].update_id;
			if (uid >= max_offset) {
				max_offset = uid + 1;
			}
			parsed++;
		}
	}

	updates->count = parsed;
	bot->offset = max_offset;

	yyjson_doc_free(doc);
	return 0;
}

void tgbot_updates_free(tgbot_updates_t *updates) {
	if (!updates) {
		return;
	}

	free(updates->items);
	updates->items = NULL;
	updates->count = 0;
}

int tgbot_get_me(const tgbot_s *bot, tgbot_me_s *me) {
	char url[URL_LEN];
	snprintf(url, sizeof(url), "%sgetMe", bot->api);

	struct memory_buffer *mb = NULL;
	int ret = tgbot_request(url, &mb, NULL);
	if (ret != 0) {
		return -1;
	}

	yyjson_doc *doc = yyjson_read(mb->data, mb->size, 0);
	free(mb->data);
	free(mb);
	if (!doc) {
		return -1;
	}

	yyjson_val *root = yyjson_doc_get_root(doc);

	yyjson_val *ok = yyjson_obj_get(root, "ok");
	if (!ok || !yyjson_is_bool(ok) || !yyjson_get_bool(ok)) {
		yyjson_doc_free(doc);
		return -1;
	}

	yyjson_val *result = yyjson_obj_get(root, "result");
	if (!result) {
		yyjson_doc_free(doc);
		return -1;
	}

	ret = tgbot_parse_me(me, result);
	yyjson_doc_free(doc);

	return ret;
}

int tgbot_send_message(const tgbot_s *bot, tgbot_message *message) {
	if (!message) {
		return -1;
	}

	tgbot_option_s options[4] = {
		{"chat_id", &message->chat_id, tgbot_opt_int64},
		{"text", (void *)message->text, tgbot_opt_string},
		{"parse_mode", (void *)message->parse_mode, tgbot_opt_string},
		{"reply_markup", (void *)message->reply_markup, tgbot_opt_inlinekeyboard},
	};

	return execute_method(bot, "sendMessage", options, opt_size(options));
}

int tgbot_edit_message_text(const tgbot_s *bot, tgbot_message *message) {
	if (!message) {
		return -1;
	}

	tgbot_option_s options[4] = {
		{"chat_id", &message->chat_id, tgbot_opt_int64},
		{"message_id", &message->message_id, tgbot_opt_int64},
		{"text", (void *)message->text, tgbot_opt_string},
		{"reply_markup", message->reply_markup, tgbot_opt_inlinekeyboard},
	};

	return execute_method(bot, "editMessageText", options, opt_size(options));
}

int tgbot_send_dice(const tgbot_s *bot, tgbot_dice *dice) {
	if (!dice) {
		return -1;
	}

	tgbot_option_s options[2] = {
		{"chat_id", &dice->chat_id, tgbot_opt_int64},
		{"emoji", (void *)dice->emoji, tgbot_opt_string},
	};

	return execute_method(bot, "sendDice", options, opt_size(options));
}

int tgbot_send_photo(const tgbot_s *bot, tgbot_photo *photo) {
	if (!photo) {
		return -1;
	}

	return execute_method_multipart(bot, "sendPhoto", photo->chat_id, photo->path, photo->caption);
}
