#ifndef TGBOT_MAIN_H
#define TGBOT_MAIN_H

#include "common.h"
#include "methods.h"
#include "types.h"

/**
 * @brief Initializes the Bot object.
 *
 * @param[out] bot The Bot object.
 * @param[in] token The Bot token (obtained from @BotFather).
 *
 * @return TGBOT_OK on success.
 */
tgbot_rc tgbot_init(tgbot *bot, char *token);

/**
 * @brief Cleans the memory.
 *
 * @param[out] bot The Bot object.
 */
void tgbot_destroy(tgbot *bot);

#endif // TGBOT_H
