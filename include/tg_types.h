#ifndef TGBOT_TG_TYPES_H
#define TGBOT_TG_TYPES_H

#include "types.h"

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
