#ifndef TGBOT_TYPES_H
#define TGBOT_TYPES_H

#include <stddef.h>
#include <stdint.h>

#define FIRSTNAME_SIZE 256
#define USERNAME_SIZE 32
#define CHAT_LASTNAME_SIZE 256
#define CHAT_TYPE_SIZE 32
#define CHAT_TEXT_SIZE 4096
#define TEXT_SIZE 200
#define URL_SIZE 200
#define CB_DATA_SIZE 64

#define API_SIZE 512
#define URL_LEN 1024

/**
 * @brief Telegram bot struct.
 */
typedef struct tgbot {
	char api[API_SIZE]; /**< Bot API url. */
	int64_t offset;		/**< Bot offset. */
} tgbot_s;

/**
 * @brief Represents update type.
 */
enum tgbot_update_type {
	MESSAGE,
	CALLBACK_QUERY,
};
typedef enum tgbot_update_type tgbot_update_type_e;

/**
 * @brief A structure to represent Bot information from getMe.
 */
typedef struct tgbot_me {
	char first_name[FIRSTNAME_SIZE];
	char username[USERNAME_SIZE];
} tgbot_me_s;

/**
 * @brief A parsed message update.
 */
typedef struct tgbot_update {
	int64_t update_id;
	int64_t message_id;
	int64_t chat_id;
	char chat_first_name[FIRSTNAME_SIZE];
	char chat_last_name[CHAT_LASTNAME_SIZE];
	char chat_username[USERNAME_SIZE];
	char chat_type[CHAT_TYPE_SIZE];
	int32_t date;
	char text[CHAT_TEXT_SIZE];
} tgbot_update_s;

/**
 * @brief A parsed callback query update.
 */
typedef struct tgbot_cbquery {
	int64_t update_id;
	int64_t message_id;
	int64_t chat_id;
	char chat_username[USERNAME_SIZE];
	int32_t date;
	char text[CHAT_TEXT_SIZE];
	char chat_instance[128];
	char data[64];
} tgbot_cbquery_s;

/**
 * @brief A single parsed update from the Telegram API.
 *
 * Check type to determine which union member is valid.
 */
typedef struct tgbot_update_result {
	tgbot_update_type_e type;
	int64_t update_id;
	union {
		tgbot_update_s message;
		tgbot_cbquery_s cbquery;
	};
} tgbot_update_result_t;

/**
 * @brief Allocated array of updates returned by tgbot_get_updates().
 *
 * Free with tgbot_updates_free().
 */
typedef struct tgbot_updates {
	tgbot_update_result_t *items;
	size_t count;
} tgbot_updates_t;

/**
 * @brief Represents a single button on an inline keyboard.
 */
typedef struct tgbot_inlinekeyboardbutton {
	char text[TEXT_SIZE];
	char url[URL_SIZE];
	char callback_data[CB_DATA_SIZE];
} tgbot_inlinekeyboardbutton_s;

/**
 * @brief Represents an inline keyboard.
 */
typedef struct tgbot_inlinekeyboard {
	size_t rows;
	size_t columns;
	tgbot_inlinekeyboardbutton_s *buttons;
} tgbot_inlinekeyboard_s;

/**
 * @brief Parameters for sendMessage / editMessageText.
 */
typedef struct tgbot_message {
	int64_t chat_id;
	int64_t message_id;
	const char *text;
	const char *parse_mode;
	tgbot_inlinekeyboard_s *reply_markup;
} tgbot_message;

/**
 * @brief Parameters for sendDice.
 */
typedef struct tgbot_dice {
	int64_t chat_id;
	const char *emoji;
} tgbot_dice;

/**
 * @brief Parameters for sendPhoto.
 */
typedef struct tgbot_photo {
	int64_t chat_id;
	const char *path;
	const char *caption;
} tgbot_photo;

tgbot_inlinekeyboard_s *tgbot_inlinekb_new(size_t rows, size_t columns);

int tgbot_inlinekb_button(tgbot_inlinekeyboard_s *keyboard, size_t row, size_t column, const char *text,
						  const char *url, const char *callback_data);

tgbot_inlinekeyboardbutton_s *tgbot_inlinekb_button_at(tgbot_inlinekeyboard_s *keyboard, size_t row, size_t column);

void tgbot_inlinekb_free(tgbot_inlinekeyboard_s *keyboard);

#endif
