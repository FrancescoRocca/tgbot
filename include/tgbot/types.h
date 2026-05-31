#ifndef TGBOT_TYPES_H
#define TGBOT_TYPES_H

#include <stddef.h>
#include <stdint.h>

#define FIRSTNAME_SIZE 256
#define USERNAME_SIZE 32

#define CHAT_LASTNAME_SIZE 256
#define CHAT_TYPE_SIZE 32
#define CHAT_TEXT_SIZE 4096

/**
 * @brief Telegram bot struct.
 */
typedef struct tgbot tgbot_s;

/**
 * @brief A structure to represent Bot information got from getMe API.
 */
typedef struct tgbot_me {
	char first_name[FIRSTNAME_SIZE]; /**< Bot's first name. */
	char username[USERNAME_SIZE];	 /**< Bot's username. */
} tgbot_me_s;

/**
 * @brief A structure to represent Update object.
 */
typedef struct tgbot_update {
	int64_t update_id;						 /**< Update id. */
	long message_id;						 /**< Message id. */
	int64_t chat_id;						 /**< Chat id. */
	char chat_first_name[FIRSTNAME_SIZE];	 /**< Chat first name. */
	char chat_last_name[CHAT_LASTNAME_SIZE]; /**< Chat last name. */
	char chat_username[USERNAME_SIZE];		 /**< Chat username. */
	char chat_type[CHAT_TYPE_SIZE];			 /**< Chat type (private/public). */
	int32_t date;							 /**< Date in unix timestamp. */
	char text[CHAT_TEXT_SIZE];				 /**< Message text. */
} tgbot_update_s;

/**
 * @brief A structure to represent CallbackQuery object.
 */
typedef struct tgbot_cbquery {
	int64_t update_id;
	long message_id;
	int64_t chat_id;
	char chat_username[USERNAME_SIZE];
	int32_t date;
	char text[CHAT_TEXT_SIZE];
	char chat_instance[128];
	char data[64]; /**> Callback data. */
} tgbot_cbquery_s;

/**
 * @brief Callback function pointer.
 */
typedef void (*Callback)(tgbot_s *bot, tgbot_cbquery_s *query);

#define TEXT_SIZE 200
#define URL_SIZE 200
#define CB_DATA_SIZE 64

/**
 * @brief Represents a single button on an inline keyboard.
 */
typedef struct tgbot_inlinekeyboardbutton {
	char text[TEXT_SIZE];			  /**< Text of the button. If empty, the button is ignored. */
	char url[URL_SIZE];				  /**< (Optional) URL to be opened when the button is pressed. */
	char callback_data[CB_DATA_SIZE]; /**< (Optional) Data sent to the bot when the button is pressed. */
} tgbot_inlinekeyboardbutton_s;

/**
 * @brief Represents an inline keyboard.
 */
typedef struct tgbot_inlinekeyboard {
	size_t rows;						   /**< Number of rows in the keyboard. */
	size_t columns;						   /**< Number of columns per row. */
	tgbot_inlinekeyboardbutton_s *buttons; /**< Array of buttons. */
} tgbot_inlinekeyboard_s;

#endif
