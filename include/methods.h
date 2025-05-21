#ifndef TGBOT_METHODS_H
#define TGBOT_METHODS_H

#include "types.h"
#include <json-c/json.h>

tgbot_rc tgbot_get_update(tgbot *bot, tgbot_update *update, Callback cbq_handler);
tgbot_rc tgbot_parse_message(tgbot *bot, tgbot_update *update, json_object *result);
tgbot_rc tgbot_parse_cbquery(tgbot *bot, tgbot_cbquery *query, json_object *result, Callback query_handler);

/* Request */
size_t write_callback(void *ptr, size_t size, size_t nmemb, char **userdata);
tgbot_rc tgbot_request(tgbot *bot, char *url, struct memory_buffer **mb, json_object *json);

/* Methods */
tgbot_rc tgbot_get_me(tgbot *bot, tgbot_me *me);
tgbot_rc tgbot_send_message(tgbot *bot, int64_t chat_id, char *text, char *parse_mode, tgbot_inlinekeyboardmarkup **reply_markup, size_t rows, size_t columns);

/* Updating Methods */
tgbot_rc tgbot_edit_message_text(tgbot *bot, int64_t chat_id, long message_id, char *text, tgbot_inlinekeyboardmarkup **keyboard, size_t rows, size_t columns);

#endif
