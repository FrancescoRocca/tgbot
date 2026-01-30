#ifndef TGBOT_MAIN_H
#define TGBOT_MAIN_H

#include "common.h"
#include "methods.h"
#include "types.h"

/**
 * @brief Create a new Bot object.
 *
 * @param[in] token The Bot token (obtained from @BotFather).
 *
 * @return Bot pointer.
 */
tgbot_s *tgbot_new(const char *token);

/**
 * @brief Cleans the memory.
 *
 * @param[out] bot The Bot object.
 */
void tgbot_free(tgbot_s *bot);

#endif // TGBOT_H
