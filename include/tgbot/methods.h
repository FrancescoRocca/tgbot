#ifndef TGBOT_METHODS_H
#define TGBOT_METHODS_H

#include "types.h"

/* Retrieve update */
int tgbot_get_update(tgbot_s *bot, tgbot_update_s *update, Callback cbq_handler);

/* Methods */
int tgbot_get_me(const tgbot_s *bot, tgbot_me_s *me);
int tgbot_send_message(const tgbot_s *bot, int64_t chat_id, const char *text, const char *parse_mode,
					   tgbot_inlinekeyboard_s *reply_markup);
int tgbot_send_dice(const tgbot_s *bot, int64_t chat_id, const char *emoji);
int tgbot_send_photo(const tgbot_s *bot, int64_t chat_id, const char *path, const char *caption);

/* Updating Methods */
int tgbot_edit_message_text(const tgbot_s *bot, int64_t chat_id, long message_id, const char *text,
							tgbot_inlinekeyboard_s *keyboard);

#endif
