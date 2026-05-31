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
	int64_t message_id;						 /**< Message id. */
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
	int64_t message_id;
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

/**
 * @brief Message struct.
 */
typedef struct tgbot_message {
	int64_t chat_id;
	int64_t message_id;
	const char *text;
	const char *parse_mode;
	tgbot_inlinekeyboard_s *reply_markup;
} tgbot_message;

/**
 * @brief Dice struct.
 */
typedef struct tgbot_dice {
	int64_t chat_id;
	const char *emoji;
} tgbot_dice;

/**
 * @brief Photo struct.
 */
typedef struct tgbot_photo {
	int64_t chat_id;
	const char *path;
	const char *caption;
} tgbot_photo;

/**
 * @brief Allocates a new inline keyboard.
 *
 * @param[in] rows Number of rows in the keyboard.
 * @param[in] columns Number of columns in each row.
 *
 * @return The pointer to the keyboard or NULL on allocation failure.
 */
tgbot_inlinekeyboard_s *tgbot_inlinekb_new(size_t rows, size_t columns);

/**
 * @brief Adds or updates a button at the specified position in the keyboard.
 *
 * @param[out] keyboard Pointer to the keyboard to modify.
 * @param[in] row Row index of the button (starting from 0).
 * @param[in] column Column index of the button (starting from 0).
 * @param[in] text Display text for the button.
 * @param[in] url Optional URL for the button.
 * @param[in] callback_data Optional callback data for the button.
 *
 * @return 0 on success.
 */
int tgbot_inlinekb_button(tgbot_inlinekeyboard_s *keyboard, size_t row, size_t column, const char *text,
						  const char *url, const char *callback_data);

/**
 * @brief Returns a pointer to the keyboard's button.
 *
 * @param[in] keyboard Keyboard.
 * @param[in] row Row index of the button.
 * @param[in] column Column index of the button.
 *
 * @return Pointer to the button, or NULL if the position is invalid.
 */
tgbot_inlinekeyboardbutton_s *tgbot_inlinekb_button_at(tgbot_inlinekeyboard_s *keyboard, size_t row, size_t column);

/**
 * @brief Frees all memory associated with the given inline keyboard.
 *
 * @param[in,out] keyboard Pointer to the keyboard structure to deallocate.
 */
void tgbot_inlinekb_free(tgbot_inlinekeyboard_s *keyboard);

#endif
