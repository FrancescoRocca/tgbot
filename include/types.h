#ifndef TGBOT_TYPES_H
#define TGBOT_TYPES_H

#include "common.h"

/**
 * @brief Represents a single button on an inline keyboard.
 */
struct tgbot_inlinekeyboardbutton_t {
	char text[200];			/**< Text of the button. If empty, the button is ignored. */
	char url[200];			/**< (Optional) URL to be opened when the button is pressed. */
	char callback_data[64]; /**< (Optional) Data sent to the bot when the button is pressed. */
};
typedef struct tgbot_inlinekeyboardbutton_t tgbot_inlinekeyboardbutton;

/**
 * @brief Represents an inline keyboard.
 */
struct tgbot_inlinekeyboard_t {
	size_t rows;								  /**< Number of rows in the keyboard. */
	size_t columns;								  /**< Number of columns per row. */
	struct tgbot_inlinekeyboardbutton_t *buttons; /**< Array of buttons. */
};
typedef struct tgbot_inlinekeyboard_t tgbot_inlinekeyboard;

/**
 * @brief Allocates a new inline keyboard.
 *
 * @param[in] rows Number of rows in the keyboard.
 * @param[in] columns Number of columns in each row.
 *
 * @return The pointer to the keyboard or NULL on allocation failure.
 */
tgbot_inlinekeyboard *tgbot_new_inlinekeyboard(size_t rows, size_t columns);

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
 * @return TGBOT_OK on success.
 */
tgbot_rc tgbot_inlinekeyboard_button(tgbot_inlinekeyboard *keyboard, size_t row, size_t column, const char *text, const char *url, const char *callback_data);

/**
 * @brief Returns a pointer to the keyboard's button.
 *
 * @param[in] keyboard Keyboard.
 * @param[in] row Row index of the button.
 * @param[in] column Column index of the button.
 *
 * @return Pointer to the button, or NULL if the position is invalid.
 */
tgbot_inlinekeyboardbutton *tgbot_inlinekeyboard_button_at(tgbot_inlinekeyboard *keyboard, size_t row, size_t column);

/**
 * @brief Frees all memory associated with the given inline keyboard.
 *
 * @param[in,out] keyboard Pointer to the keyboard structure to deallocate.
 */
void tgbot_destroy_inlinekeyboard(tgbot_inlinekeyboard *keyboard);

#endif
