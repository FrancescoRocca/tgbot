#ifndef TGBOT_MAIN_H
#define TGBOT_MAIN_H

#include "common.h"
#include "methods.h"
#include "types.h"

/**
 * Initializes the Bot object
 * @param[out] bot The Bot object
 * @param[in] token The Bot token (obtained from @BotFather)
 */
tgbot_rc tgbot_init(tgbot *bot, char *token);

/**
 * Cleans the memory
 * @param bot The Bot object
 */
void tgbot_destroy(tgbot *bot);

#endif // TGBOT_H
