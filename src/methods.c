#include <curl/curl.h>
#include <string.h>

#include "json.h"
#include "methods.h"

tgbot_rc tgbot_get_update(tgbot *bot, tgbot_update *update, Callback cbq_handler) {
	char url[1024];

	/* Clear Update */
	memset(update, 0, sizeof(tgbot_update));

	int limit = 1;
	int timeout = 30;
	tgbot_json_option options[3] = {
		{"offset", &bot->offset, tgbot_opt_int64},
		{"limit", &limit, tgbot_opt_int},
		{"timeout", &timeout, tgbot_opt_int},
	};
	json_object *rjson = tgbot_json_builder(options, 3);

	snprintf(url, sizeof(url), "%sgetUpdates", bot->api);

	struct memory_buffer *mb;
	tgbot_rc ret = tgbot_request(url, &mb, rjson);
	json_object_put(rjson);
	if (ret != TGBOT_OK) {
		free(mb->data);
		free(mb);

		return TGBOT_GETUPDATES_ERROR;
	}

	json_object *json = json_tokener_parse(mb->data);
	free(mb->data);
	free(mb);

	json_object *ok = json_object_object_get(json, "ok");
	if (!json_object_is_type(ok, json_type_boolean) || !json_object_get_boolean(ok)) {
		json_object_put(json);

		return TGBOT_TELEGRAM_OK_ERROR;
	}

	json_object *results = json_object_object_get(json, "result");
	size_t results_len = json_object_array_length(results);

	if (results_len == 0) {
		json_object_put(json);

		return TGBOT_OK;
	}

	/* Check if it is a Message or a CallbackQuery*/
	json_object *result = json_object_array_get_idx(results, 0);
	json_object *message = json_object_object_get(result, "message");
	if (message) {
		tgbot_parse_message(bot, update, result);
	} else if (cbq_handler != NULL) {
		tgbot_cbquery query;
		tgbot_parse_cbquery(bot, &query, result, cbq_handler);
	}

	json_object_put(json);

	return TGBOT_OK;
}

tgbot_rc tgbot_parse_message(tgbot *bot, tgbot_update *update, json_object *result) {
	json_object *update_id = json_object_object_get(result, "update_id");
	bot->offset = json_object_get_int(update_id) + 1;
	update->update_id = json_object_get_int(update_id);

	json_object *message = json_object_object_get(result, "message");
	json_object *message_id = json_object_object_get(message, "message_id");
	if (message_id) {
		update->message_id = json_object_get_int(message_id);
	}

	json_object *chat = json_object_object_get(message, "chat");
	json_object *chat_id = json_object_object_get(chat, "id");
	if (chat_id) {
		update->chat_id = json_object_get_int64(chat_id);
	}

	json_object *chat_first_name = json_object_object_get(chat, "first_name");
	if (chat_first_name) {
		strncpy(update->chat_first_name, json_object_get_string(chat_first_name), sizeof(update->chat_first_name) - 1);
	}

	json_object *chat_last_name = json_object_object_get(chat, "last_name");
	if (chat_last_name) {
		strncpy(update->chat_last_name, json_object_get_string(chat_last_name), sizeof(update->chat_last_name) - 1);
	}

	json_object *chat_username = json_object_object_get(chat, "username");
	if (chat_username != NULL) {
		strncpy(update->chat_username, json_object_get_string(chat_username), sizeof(update->chat_username) - 1);
	}

	json_object *chat_type = json_object_object_get(chat, "type");
	if (chat_type) {
		strncpy(update->chat_type, json_object_get_string(chat_type), sizeof(update->chat_type) - 1);
	}

	json_object *date = json_object_object_get(message, "date");
	if (date) {
		update->date = json_object_get_int(date);
	}

	json_object *text = json_object_object_get(message, "text");
	if (text) {
		strncpy(update->text, json_object_get_string(text), sizeof(update->text) - 1);
	}

	return TGBOT_OK;
}

tgbot_rc tgbot_parse_cbquery(tgbot *bot, tgbot_cbquery *query, json_object *result, Callback cbq_handler) {
	json_object *update_id = json_object_object_get(result, "update_id");
	bot->offset = json_object_get_int(update_id) + 1;
	query->update_id = json_object_get_int(update_id);

	json_object *callback_query = json_object_object_get(result, "callback_query");
	json_object *message = json_object_object_get(callback_query, "message");
	json_object *message_id = json_object_object_get(message, "message_id");
	if (message_id) {
		query->message_id = json_object_get_int(message_id);
	}

	json_object *chat = json_object_object_get(message, "chat");
	json_object *chat_id = json_object_object_get(chat, "id");
	if (chat_id) {
		query->chat_id = json_object_get_int64(chat_id);
	}

	json_object *chat_username = json_object_object_get(chat, "username");
	if (chat_username) {
		strncpy(query->chat_username, json_object_get_string(chat_username), sizeof(query->chat_username) - 1);
	}

	json_object *date = json_object_object_get(message, "date");
	if (date) {
		query->date = json_object_get_int(date);
	}

	json_object *text = json_object_object_get(message, "text");
	if (text) {
		strncpy(query->text, json_object_get_string(text), sizeof(query->text) - 1);
	}

	json_object *chat_instance = json_object_object_get(callback_query, "chat_instance");
	if (chat_instance) {
		strncpy(query->chat_instance, json_object_get_string(chat_instance), sizeof(query->chat_instance) - 1);
	}

	json_object *data = json_object_object_get(callback_query, "data");
	if (data) {
		strncpy(query->data, json_object_get_string(data), sizeof(query->data) - 1);
	}

	cbq_handler(bot, query);

	return TGBOT_OK;
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, char **userdata) {
	size_t real_size = size * nmemb;
	struct memory_buffer *mem = (struct memory_buffer *)userdata;

	mem->data = realloc(mem->data, mem->size + real_size + 1);
	memcpy(&(mem->data[mem->size]), ptr, real_size);
	mem->size += real_size;
	mem->data[mem->size] = '\0';

	return real_size;
}

size_t discard_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	return size * nmemb;
}

tgbot_rc tgbot_request(const char *url, struct memory_buffer **mb, json_object *json) {
	CURL *curl = curl_easy_init();
	if (!curl) {
		return TGBOT_REQUEST_ERROR;
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
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
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
		if (mb != NULL && *mb) {
			free((*mb)->data);
			free(*mb);
			*mb = NULL;
		}

		return TGBOT_REQUEST_ERROR;
	}

	curl_easy_cleanup(curl);

	return TGBOT_OK;
}

tgbot_rc tgbot_get_me(tgbot *bot, tgbot_me *me) {
	char url[1024];
	snprintf(url, sizeof(url), "%sgetMe", bot->api);

	struct memory_buffer *mb;
	tgbot_rc ret = tgbot_request(url, &mb, NULL);
	if (ret != TGBOT_OK) {
		free(mb->data);
		free(mb);

		return TGBOT_GETME_ERROR;
	}

	json_object *json = json_tokener_parse(mb->data);
	free(mb->data);
	free(mb);

	json_object *ok = json_object_object_get(json, "ok");
	if (!json_object_get_boolean(ok)) {
		json_object_put(json);

		return TGBOT_GETME_ERROR;
	}

	json_object *result = json_object_object_get(json, "result");
	json_object *first_name = json_object_object_get(result, "first_name");
	strncpy(me->first_name, json_object_get_string(first_name), sizeof(me->first_name) - 1);
	json_object *username = json_object_object_get(result, "username");
	strncpy(me->username, json_object_get_string(username), sizeof(me->username) - 1);

	json_object_put(json);

	return TGBOT_OK;
}

tgbot_rc tgbot_send_message(tgbot *bot, int64_t chat_id, const char *text, const char *parse_mode, tgbot_inlinekeyboard *keyboard) {
	char url[1024];

	tgbot_json_option options[4] = {
		{"chat_id", &chat_id, tgbot_opt_int64},
		{"text", (void *)text, tgbot_opt_string},
		{"parse_mode", (void *)parse_mode, tgbot_opt_string},
		{"reply_markup", keyboard, tgbot_opt_inlinekeyboard},
	};
	json_object *rjson = tgbot_json_builder(options, 4);

	snprintf(url, sizeof(url), "%ssendMessage", bot->api);

	tgbot_rc ret = tgbot_request(url, NULL, rjson);
	json_object_put(rjson);

	if (ret != TGBOT_OK) {
		return TGBOT_SENDMESSAGE_ERROR;
	}

	return TGBOT_OK;
}

tgbot_rc tgbot_edit_message_text(tgbot *bot, int64_t chat_id, long message_id, const char *text, tgbot_inlinekeyboard *keyboard) {
	char url[1024];

	tgbot_json_option options[4] = {
		{"chat_id", &chat_id, tgbot_opt_int64},
		{"message_id", &message_id, tgbot_opt_int},
		{"text", (void *)text, tgbot_opt_string},
		{"reply_markup", keyboard, tgbot_opt_inlinekeyboard},
	};
	json_object *rjson = tgbot_json_builder(options, 4);

	snprintf(url, sizeof(url), "%seditMessageText", bot->api);

	tgbot_rc ret = tgbot_request(url, NULL, rjson);
	json_object_put(rjson);

	if (ret != TGBOT_OK) {
		return TGBOT_EDITMESSAGETEXT_ERROR;
	}

	return TGBOT_OK;
}

tgbot_rc tgbot_send_dice(tgbot *bot, int64_t chat_id, const char *emoji) {
	char url[1024];

	tgbot_json_option options[2] = {
		{"chat_id", &chat_id, tgbot_opt_int64},
		{"emoji", (void *)emoji, tgbot_opt_string},
	};
	json_object *rjson = tgbot_json_builder(options, 2);

	snprintf(url, sizeof(url), "%ssendDice", bot->api);

	tgbot_rc ret = tgbot_request(url, NULL, rjson);
	json_object_put(rjson);

	if (ret != TGBOT_OK) {
		return TGBOT_SENDDICE_ERROR;
	}

	return TGBOT_OK;
}
