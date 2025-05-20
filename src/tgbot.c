#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tgbot.h"

tgbot_rc tgbot_init(tgbot *bot, char *token) {
	curl_global_init(CURL_GLOBAL_DEFAULT);

	snprintf(bot->api, sizeof(bot->api), "https://api.telegram.org/bot%s/", token);
	snprintf(bot->token, sizeof(bot->token), "%s", token);
	bot->offset = 0;
	bot->curl = curl_easy_init();

	if (!bot->curl) {
		return TGBOT_INIT_ERROR;
	}

	return TGBOT_OK;
}

void tgbot_destroy(tgbot *bot) {
	if (bot->curl) {
		curl_easy_cleanup(bot->curl);
	}
	curl_global_cleanup();
}

tgbot_rc tgbot_allocate_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup ***keyboard, size_t rows, size_t columns) {
	*keyboard = (tgbot_inlinekeyboardmarkup **)malloc(rows * sizeof(tgbot_inlinekeyboardmarkup *));
	for (size_t i = 0; i < rows; ++i) {
		(*keyboard)[i] = (tgbot_inlinekeyboardmarkup *)malloc(columns * sizeof(tgbot_inlinekeyboardmarkup));
	}

	return TGBOT_OK;
}

tgbot_rc tgbot_deallocate_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup **keyboard, size_t rows) {
	for (size_t i = 0; i < rows; ++i) {
		free(keyboard[i]);
	}

	free(keyboard);

	return TGBOT_OK;
}

json_object *tgbot_new_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup **keyboard, size_t rows, size_t columns) {
	json_object *reply_markup = json_object_new_object();
	json_object *inline_keyboard_array = json_object_new_array();

	for (size_t i = 0; i < rows; ++i) {
		json_object *row = json_object_new_array();
		for (size_t j = 0; j < columns; ++j) {
			if (strcmp(keyboard[i][j].text, "") == 0) {
				continue;
			}

			json_object *button = json_object_new_object();
			json_object_object_add(button, "text", json_object_new_string(keyboard[i][j].text));
			json_object_object_add(button, "url", json_object_new_string(keyboard[i][j].url));
			json_object_object_add(button, "callback_data", json_object_new_string(keyboard[i][j].callback_data));

			json_object_array_add(row, button);
		}
		json_object_array_add(inline_keyboard_array, row);
	}

	json_object_object_add(reply_markup, "inline_keyboard", inline_keyboard_array);

	return reply_markup;
}

tgbot_rc tgbot_get_update(tgbot *bot, tgbot_update *update, Callback cbq_handler) {
	char url[1024];

	/* Clear Update */
	memset(update, 0, sizeof(tgbot_update));

	json_object *rjson = json_object_new_object();
	json_object_object_add(rjson, "offset", json_object_new_int64(bot->offset));
	json_object_object_add(rjson, "limit", json_object_new_int(1));
	json_object_object_add(rjson, "timeout", json_object_new_int(30));

	snprintf(url, sizeof(url), "%sgetUpdates", bot->api);

	struct memory_buffer *mb;
	tgbot_rc ret = tgbot_request(bot, url, &mb, rjson);
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
	} else {
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
	update->message_id = json_object_get_int(message_id);

	json_object *chat = json_object_object_get(message, "chat");
	json_object *chat_id = json_object_object_get(chat, "id");
	update->chat_id = json_object_get_int64(chat_id);

	json_object *chat_first_name = json_object_object_get(chat, "first_name");
	strncpy(update->chat_first_name, json_object_get_string(chat_first_name), sizeof(update->chat_first_name));
	json_object *chat_last_name = json_object_object_get(chat, "last_name");
	if (chat_last_name != NULL) {
		strncpy(update->chat_last_name, json_object_get_string(chat_last_name), sizeof(update->chat_last_name));
	} else {
		update->chat_last_name[0] = '\0';
	}
	json_object *chat_username = json_object_object_get(chat, "username");
	if (chat_username != NULL) {
		strncpy(update->chat_username, json_object_get_string(chat_username), sizeof(update->chat_username));
	} else {
		update->chat_username[0] = '\0';
	}
	json_object *chat_type = json_object_object_get(chat, "type");
	strncpy(update->chat_type, json_object_get_string(chat_type), sizeof(update->chat_type));

	json_object *date = json_object_object_get(message, "date");
	update->date = json_object_get_int(date);

	json_object *text = json_object_object_get(message, "text");
	strncpy(update->text, json_object_get_string(text), sizeof(update->text));

	return TGBOT_OK;
}

tgbot_rc tgbot_parse_cbquery(tgbot *bot, tgbot_cbquery *query, json_object *result, Callback cbq_handler) {
	json_object *update_id = json_object_object_get(result, "update_id");
	bot->offset = json_object_get_int(update_id) + 1;
	query->update_id = json_object_get_int(update_id);

	json_object *callback_query = json_object_object_get(result, "callback_query");
	json_object *message = json_object_object_get(callback_query, "message");
	json_object *message_id = json_object_object_get(message, "message_id");
	query->message_id = json_object_get_int(message_id);
	json_object *chat = json_object_object_get(message, "chat");
	json_object *chat_id = json_object_object_get(chat, "id");
	query->chat_id = json_object_get_int64(chat_id);
	json_object *chat_username = json_object_object_get(chat, "username");
	/* TODO: add NULL checks */
	strncpy(query->chat_username, json_object_get_string(chat_username), sizeof(query->chat_username));
	json_object *date = json_object_object_get(message, "date");
	query->date = json_object_get_int(date);
	json_object *text = json_object_object_get(message, "text");
	strncpy(query->text, json_object_get_string(text), sizeof(query->text));
	json_object *chat_instance = json_object_object_get(callback_query, "chat_instance");
	strncpy(query->chat_instance, json_object_get_string(chat_instance), sizeof(query->chat_instance));
	json_object *data = json_object_object_get(callback_query, "data");
	strncpy(query->data, json_object_get_string(data), sizeof(query->data));

	cbq_handler(bot, query);

	return TGBOT_OK;
}

tgbot_rc tgbot_get_me(tgbot *bot, tgbot_me *me) {
	char url[1024];
	snprintf(url, sizeof(url), "%sgetMe", bot->api);

	struct memory_buffer *mb;
	tgbot_rc ret = tgbot_request(bot, url, &mb, NULL);
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
	strncpy(me->first_name, json_object_get_string(first_name), sizeof(me->first_name));
	json_object *username = json_object_object_get(result, "username");
	strncpy(me->username, json_object_get_string(username), sizeof(me->username));

	json_object_put(json);

	return TGBOT_OK;
}

tgbot_rc tgbot_send_message(tgbot *bot, int64_t chat_id, char *text, char *parse_mode, tgbot_inlinekeyboardmarkup **keyboard, size_t rows, size_t columns) {
	char url[1024];

	json_object *rjson = json_object_new_object();
	json_object_object_add(rjson, "chat_id", json_object_new_int64(chat_id));
	json_object_object_add(rjson, "text", json_object_new_string(text));
	json_object_object_add(rjson, "parse_mode", json_object_new_string(parse_mode));

	if (keyboard != NULL) {
		json_object *reply_markup = tgbot_new_inlinekeyboardmarkup(keyboard, rows, columns);
		json_object_object_add(rjson, "reply_markup", reply_markup);
	}

	snprintf(url, sizeof(url), "%ssendMessage", bot->api);

	struct memory_buffer *mb;
	tgbot_rc ret = tgbot_request(bot, url, &mb, rjson);
	json_object_put(rjson);
	if (ret != TGBOT_OK) {
		free(mb->data);
		free(mb);

		return TGBOT_SENDMESSAGE_ERROR;
	}

	free(mb->data);
	free(mb);

	return TGBOT_OK;
}

tgbot_rc tgbot_edit_message_text(tgbot *bot, int64_t chat_id, long message_id, char *text, tgbot_inlinekeyboardmarkup **keyboard, size_t rows, size_t columns) {
	char url[1024];

	json_object *rjson = json_object_new_object();
	json_object_object_add(rjson, "chat_id", json_object_new_int64(chat_id));
	json_object_object_add(rjson, "message_id", json_object_new_int(message_id));
	json_object_object_add(rjson, "text", json_object_new_string(text));

	snprintf(url, sizeof(url), "%seditMessageText", bot->api);

	if (keyboard != NULL) {
		json_object *reply_markup = tgbot_new_inlinekeyboardmarkup(keyboard, rows, columns);
		json_object_object_add(rjson, "reply_markup", reply_markup);
	}

	struct memory_buffer *mb;
	tgbot_request(bot, url, &mb, rjson);
	json_object_put(rjson);
	free(mb->data);
	free(mb);

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

tgbot_rc tgbot_request(tgbot *bot, char *url, struct memory_buffer **mb, json_object *json) {
	const char *json_string = NULL;
	*mb = calloc(1, sizeof(struct memory_buffer));

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(bot->curl, CURLOPT_URL, url);
	curl_easy_setopt(bot->curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(bot->curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(bot->curl, CURLOPT_WRITEDATA, *mb);
	curl_easy_setopt(bot->curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(bot->curl, CURLOPT_TCP_KEEPALIVE, 30L);

	if (json != NULL) {
		json_string = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PLAIN);
		curl_easy_setopt(bot->curl, CURLOPT_POSTFIELDS, json_string);
	}

	CURLcode res = curl_easy_perform(bot->curl);

	curl_slist_free_all(headers);

	if (res != CURLE_OK) {
		free((*mb)->data);
		free(*mb);
		*mb = NULL;

		return TGBOT_REQUEST_ERROR;
	}

	return TGBOT_OK;
}
