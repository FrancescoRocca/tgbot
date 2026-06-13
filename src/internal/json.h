#ifndef TGBOT_INTERNAL_JSON_H
#define TGBOT_INTERNAL_JSON_H

#include <stdbool.h>
#include <tgbot/types.h>

/**
 * @brief Option type tag for json_builder().
 */
enum tgbot_opt_type {
	tgbot_opt_int32,
	tgbot_opt_int64,
	tgbot_opt_string,
	tgbot_opt_inlinekeyboard,
};
typedef enum tgbot_opt_type tgbot_opt_type_e;

/**
 * @brief A key-value option passed to json_builder().
 */
struct tgbot_option {
	char key[32];
	void *value;
	tgbot_opt_type_e type;
};
typedef struct tgbot_option tgbot_option_s;

/**
 * @brief Build a JSON request string from an array of options.
 *
 * @param[in] options Array of option key-value pairs.
 * @param[in] optionslen Number of options.
 *
 * @return Allocated JSON string (caller must free), or NULL on failure.
 */
char *json_builder(tgbot_option_s *options, size_t optionslen);

#endif
