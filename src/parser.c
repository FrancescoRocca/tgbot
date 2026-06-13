#include <stdio.h>
#include <string.h>

#include "internal/parser.h"

static int parse_chat(tgbot_update_s *update, yyjson_val *chat) {
	yyjson_val *chat_id = yyjson_obj_get(chat, "id");
	if (chat_id) {
		update->chat_id = yyjson_get_sint(chat_id);
	}

	const char *first_name = yyjson_get_str(yyjson_obj_get(chat, "first_name"));
	if (first_name) {
		snprintf(update->chat_first_name, sizeof(update->chat_first_name), "%s", first_name);
	}

	const char *last_name = yyjson_get_str(yyjson_obj_get(chat, "last_name"));
	if (last_name) {
		snprintf(update->chat_last_name, sizeof(update->chat_last_name), "%s", last_name);
	}

	const char *username = yyjson_get_str(yyjson_obj_get(chat, "username"));
	if (username) {
		snprintf(update->chat_username, sizeof(update->chat_username), "%s", username);
	}

	const char *type = yyjson_get_str(yyjson_obj_get(chat, "type"));
	if (type) {
		snprintf(update->chat_type, sizeof(update->chat_type), "%s", type);
	}

	return 0;
}

static int parse_message(tgbot_update_s *update, yyjson_val *message) {
	yyjson_val *message_id = yyjson_obj_get(message, "message_id");
	if (message_id) {
		update->message_id = yyjson_get_sint(message_id);
	}

	yyjson_val *chat = yyjson_obj_get(message, "chat");
	if (chat) {
		parse_chat(update, chat);
	}

	yyjson_val *date = yyjson_obj_get(message, "date");
	if (date) {
		update->date = (int32_t)yyjson_get_int(date);
	}

	const char *text = yyjson_get_str(yyjson_obj_get(message, "text"));
	if (text) {
		snprintf(update->text, sizeof(update->text), "%s", text);
	}

	return 0;
}

static int parse_cbquery(tgbot_cbquery_s *query, yyjson_val *cb_query) {
	yyjson_val *message = yyjson_obj_get(cb_query, "message");
	if (!message) {
		return -1;
	}

	yyjson_val *message_id = yyjson_obj_get(message, "message_id");
	if (message_id) {
		query->message_id = yyjson_get_sint(message_id);
	}

	yyjson_val *chat = yyjson_obj_get(message, "chat");
	if (chat) {
		yyjson_val *chat_id = yyjson_obj_get(chat, "id");
		if (chat_id) {
			query->chat_id = yyjson_get_sint(chat_id);
		}

		const char *username = yyjson_get_str(yyjson_obj_get(chat, "username"));
		if (username) {
			snprintf(query->chat_username, sizeof(query->chat_username), "%s", username);
		}
	}

	yyjson_val *date = yyjson_obj_get(message, "date");
	if (date) {
		query->date = (int32_t)yyjson_get_int(date);
	}

	const char *text = yyjson_get_str(yyjson_obj_get(message, "text"));
	if (text) {
		snprintf(query->text, sizeof(query->text), "%s", text);
	}

	const char *chat_instance = yyjson_get_str(yyjson_obj_get(cb_query, "chat_instance"));
	if (chat_instance) {
		snprintf(query->chat_instance, sizeof(query->chat_instance), "%s", chat_instance);
	}

	const char *data = yyjson_get_str(yyjson_obj_get(cb_query, "data"));
	if (data) {
		snprintf(query->data, sizeof(query->data), "%s", data);
	}

	return 0;
}

int tgbot_parse_update(tgbot_update_result_t *result, yyjson_val *val) {
	if (!result || !val) {
		return -1;
	}

	yyjson_val *update_id = yyjson_obj_get(val, "update_id");
	if (!update_id) {
		return -1;
	}

	result->update_id = yyjson_get_sint(update_id);

	yyjson_val *message = yyjson_obj_get(val, "message");
	if (message) {
		result->type = MESSAGE;
		memset(&result->message, 0, sizeof(result->message));
		result->message.update_id = result->update_id;
		return parse_message(&result->message, message);
	}

	yyjson_val *cb_query = yyjson_obj_get(val, "callback_query");
	if (cb_query) {
		result->type = CALLBACK_QUERY;
		memset(&result->cbquery, 0, sizeof(result->cbquery));
		result->cbquery.update_id = result->update_id;
		return parse_cbquery(&result->cbquery, cb_query);
	}

	return -1;
}

int tgbot_parse_me(tgbot_me_s *me, yyjson_val *result) {
	if (!me || !result) {
		return -1;
	}

	const char *first_name = yyjson_get_str(yyjson_obj_get(result, "first_name"));
	if (first_name) {
		snprintf(me->first_name, sizeof(me->first_name), "%s", first_name);
	}

	const char *username = yyjson_get_str(yyjson_obj_get(result, "username"));
	if (username) {
		snprintf(me->username, sizeof(me->username), "%s", username);
	}

	return 0;
}
