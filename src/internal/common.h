#ifndef TGBOT_INTERNAL_COMMON_H
#define TGBOT_INTERNAL_COMMON_H

#include "types.h"
#include <curl/curl.h>
#include <stdint.h>

#define TOKEN_SIZE 128
#define API_SIZE 512
#define URL_LEN 1024

/**
 * @brief A structure used to get curl response.
 */
struct memory_buffer {
	char *data;
	size_t size;
};

/**
 * @brief A structure to represent bot object.
 */
typedef struct tgbot {
	char token[TOKEN_SIZE]; /**< Bot token. */
	char api[API_SIZE];		/**< Bot API url. */
	int64_t offset;			/**< Bot offset. */
} tgbot_s;

enum tgbot_opt_type {
	tgbot_opt_int,
	tgbot_opt_int64,
	tgbot_opt_string,
	tgbot_opt_inlinekeyboard,
	tgbot_opt_null,
};
typedef enum tgbot_opt_type tgbot_opt_type_e;

struct tgbot_option {
	char key[32];
	void *value;
	tgbot_opt_type_e type;
};
typedef struct tgbot_option tgbot_option_s;

#endif
