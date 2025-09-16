#ifndef TGBOT_METHODS_H
#define TGBOT_METHODS_H

#include "types.h"
#include <json-c/json.h>

tgbot_rc tgbot_get_update(tgbot_s *bot, tgbot_update_s *update, Callback cbq_handler);
tgbot_rc tgbot_parse_message(tgbot_s *bot, tgbot_update_s *update, json_object *result);
tgbot_rc tgbot_parse_cbquery(tgbot_s *bot, tgbot_cbquery_s *query, json_object *result, Callback query_handler);

/* Request */
tgbot_rc tgbot_request(const char *url, struct memory_buffer **mb, json_object *json);

/* Methods */
tgbot_rc tgbot_get_me(tgbot_s *bot, tgbot_me_s *me);
tgbot_rc tgbot_send_message(tgbot_s *bot, int64_t chat_id, const char *text, const char *parse_mode, tgbot_inlinekeyboard_s *reply_markup);
tgbot_rc tgbot_send_dice(tgbot_s *bot, int64_t chat_id, const char *emoji);

/* Updating Methods */
tgbot_rc tgbot_edit_message_text(tgbot_s *bot, int64_t chat_id, long message_id, const char *text, tgbot_inlinekeyboard_s *keyboard);

#endif
