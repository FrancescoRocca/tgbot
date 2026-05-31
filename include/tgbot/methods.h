#ifndef TGBOT_METHODS_H
#define TGBOT_METHODS_H

#include "types.h"

/* Retrieve update */
int tgbot_get_update(tgbot_s *bot, tgbot_update_s *update, Callback cbq_handler);

/* Methods */
int tgbot_get_me(const tgbot_s *bot, tgbot_me_s *me);
int tgbot_send_message(const tgbot_s *bot, tgbot_message *message);
int tgbot_send_dice(const tgbot_s *bot, tgbot_dice *dice);
int tgbot_send_photo(const tgbot_s *bot, tgbot_photo *photo);

/* Updating Methods */
int tgbot_edit_message_text(const tgbot_s *bot, tgbot_message *message);

#endif
