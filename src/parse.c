#include "internal/parse.h"
#include "internal/common.h"

int tgbot_parse_message(tgbot_s *bot, tgbot_update_s *update, json_object *result) {
	if (!bot || !update || !result) {
		return -1;
	}

	json_object *update_id = json_object_object_get(result, "update_id");
	if (!update_id) {
		return -1;
	}

	bot->offset = json_object_get_int(update_id) + 1;
	update->update_id = json_object_get_int(update_id);

	json_object *message = json_object_object_get(result, "message");
	if (!message) {
		return -1;
	}

	json_object *message_id = json_object_object_get(message, "message_id");
	if (message_id) {
		update->message_id = json_object_get_int64(message_id);
	}

	json_object *chat = json_object_object_get(message, "chat");
	if (chat) {
		json_object *chat_id = json_object_object_get(chat, "id");
		if (chat_id) {
			update->chat_id = json_object_get_int64(chat_id);
		}

		json_object *chat_first_name = json_object_object_get(chat, "first_name");
		if (chat_first_name) {
			snprintf(update->chat_first_name, sizeof(update->chat_first_name), "%s",
					 json_object_get_string(chat_first_name));
		}

		json_object *chat_last_name = json_object_object_get(chat, "last_name");
		if (chat_last_name) {
			snprintf(update->chat_last_name, sizeof(update->chat_last_name), "%s",
					 json_object_get_string(chat_last_name));
		}

		json_object *chat_username = json_object_object_get(chat, "username");
		if (chat_username) {
			snprintf(update->chat_username, sizeof(update->chat_username), "%s", json_object_get_string(chat_username));
		}

		json_object *chat_type = json_object_object_get(chat, "type");
		if (chat_type) {
			snprintf(update->chat_type, sizeof(update->chat_type), "%s", json_object_get_string(chat_type));
		}
	}

	json_object *date = json_object_object_get(message, "date");
	if (date) {
		update->date = json_object_get_int(date);
	}

	json_object *text = json_object_object_get(message, "text");
	if (text) {
		snprintf(update->text, sizeof(update->text), "%s", json_object_get_string(text));
	}

	return 0;
}

int tgbot_parse_cbquery(tgbot_s *bot, tgbot_cbquery_s *query, json_object *result, Callback cbq_handler) {
	if (!bot || !query || !result || !cbq_handler) {
		return -1;
	}

	json_object *update_id = json_object_object_get(result, "update_id");
	if (!update_id) {
		return -1;
	}

	bot->offset = json_object_get_int(update_id) + 1;
	query->update_id = json_object_get_int(update_id);

	json_object *callback_query = json_object_object_get(result, "callback_query");
	if (!callback_query) {
		return -1;
	}

	json_object *message = json_object_object_get(callback_query, "message");
	if (!message) {
		return -1;
	}

	json_object *message_id = json_object_object_get(message, "message_id");
	if (message_id) {
		query->message_id = json_object_get_int64(message_id);
	}

	json_object *chat = json_object_object_get(message, "chat");
	if (chat) {
		json_object *chat_id = json_object_object_get(chat, "id");
		if (chat_id) {
			query->chat_id = json_object_get_int64(chat_id);
		}

		json_object *chat_username = json_object_object_get(chat, "username");
		if (chat_username) {
			snprintf(query->chat_username, sizeof(query->chat_username), "%s", json_object_get_string(chat_username));
		}
	}

	json_object *date = json_object_object_get(message, "date");
	if (date) {
		query->date = json_object_get_int(date);
	}

	json_object *text = json_object_object_get(message, "text");
	if (text) {
		snprintf(query->text, sizeof(query->text), "%s", json_object_get_string(text));
	}

	json_object *chat_instance = json_object_object_get(callback_query, "chat_instance");
	if (chat_instance) {
		snprintf(query->chat_instance, sizeof(query->chat_instance), "%s", json_object_get_string(chat_instance));
	}

	json_object *data = json_object_object_get(callback_query, "data");
	if (data) {
		snprintf(query->data, sizeof(query->data), "%s", json_object_get_string(data));
	}

	cbq_handler(bot, query);

	return 0;
}
