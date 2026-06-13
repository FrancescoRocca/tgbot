#ifndef TGBOT_INTERNAL_PARSER_H
#define TGBOT_INTERNAL_PARSER_H

#include <tgbot/types.h>
#include <yyjson.h>

/**
 * @brief Parse a single update JSON value into a result struct.
 *
 * @param[out] result Result struct to fill.
 * @param[in] val yyjson value representing a single update object.
 *
 * @return 0 on success, -1 on failure.
 */
int tgbot_parse_update(tgbot_update_result_t *result, yyjson_val *val);

/**
 * @brief Parse the "result" object from a getMe response.
 *
 * @param[out] me Bot info struct to fill.
 * @param[in] val yyjson value representing the "result" object.
 *
 * @return 0 on success, -1 on failure.
 */
int tgbot_parse_me(tgbot_me_s *me, yyjson_val *val);

#endif
