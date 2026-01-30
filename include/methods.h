#ifndef TGBOT_METHODS_H
#define TGBOT_METHODS_H

#include "types.h"
#include <json-c/json.h>

/* Retrieve update */
tgbot_rc tgbot_get_update(tgbot_s *bot, tgbot_update_s *update, Callback cbq_handler);

/* Methods */
tgbot_rc tgbot_get_me(tgbot_s *bot, tgbot_me_s *me);
tgbot_rc tgbot_send_message(tgbot_s *bot, int64_t chat_id, const char *text, const char *parse_mode,
							tgbot_inlinekeyboard_s *reply_markup);
tgbot_rc tgbot_send_dice(tgbot_s *bot, int64_t chat_id, const char *emoji);

/* Updating Methods */
tgbot_rc tgbot_edit_message_text(tgbot_s *bot, int64_t chat_id, long message_id, const char *text,
								 tgbot_inlinekeyboard_s *keyboard);

#endif
