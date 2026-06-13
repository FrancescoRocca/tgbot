#ifndef TGBOT_METHODS_H
#define TGBOT_METHODS_H

#include <tgbot/types.h>

/**
 * @brief Fetch all pending updates from the Telegram API.
 *
 * @param[in,out] bot Bot instance (offset is advanced on success).
 * @param[out] updates Struct to populate.
 *
 * @return 0 on success, -1 on failure.
 */
int tgbot_get_updates(tgbot_s *bot, tgbot_updates_t *updates);

/**
 * @brief Free the tgbot_updates_t.
 */
void tgbot_updates_free(tgbot_updates_t *updates);

/**
 * @brief Retrieve basic information about the bot.
 */
int tgbot_get_me(const tgbot_s *bot, tgbot_me_s *me);

/**
 * @brief Send a text message.
 */
int tgbot_send_message(const tgbot_s *bot, tgbot_message *message);

/**
 * @brief Send a dice.
 */
int tgbot_send_dice(const tgbot_s *bot, tgbot_dice *dice);

/**
 * @brief Send a photo.
 */
int tgbot_send_photo(const tgbot_s *bot, tgbot_photo *photo);

/**
 * @brief Edit a text message.
 */
int tgbot_edit_message_text(const tgbot_s *bot, tgbot_message *message);

#endif
