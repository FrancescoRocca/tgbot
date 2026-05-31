#ifndef TGBOT_MAIN_H
#define TGBOT_MAIN_H

#include "types.h"
#include <stdint.h>

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
 * Note: If you're using curl it will call curl_global_cleanup().
 * Maybe I'll change the API.
 *
 * @param[out] bot The Bot object.
 */
void tgbot_free(tgbot_s *bot);

#endif
