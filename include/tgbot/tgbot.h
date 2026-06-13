#ifndef TGBOT_H
#define TGBOT_H

#include <tgbot/types.h>

/**
 * @brief Create a new Bot object.
 *
 * @param[in] token The Bot token (obtained from @BotFather).
 *
 * @return Bot pointer, or NULL on failure.
 * @note Call curl_global_init() before using the bot.
 */
tgbot_s *tgbot_new(const char *token);

/**
 * @brief Free a Bot object.
 */
void tgbot_free(tgbot_s *bot);

#endif
