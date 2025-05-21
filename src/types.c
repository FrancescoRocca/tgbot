#include <stdlib.h>
#include <string.h>

#include "types.h"

tgbot_inlinekeyboard *tgbot_new_inlinekeyboard(size_t rows, size_t columns) {
	tgbot_inlinekeyboard *keyboard = (tgbot_inlinekeyboard *)malloc(sizeof(tgbot_inlinekeyboard));
	if (!keyboard)
		return NULL;
	keyboard->rows = rows;
	keyboard->columns = columns;
	keyboard->buttons = (tgbot_inlinekeyboardbutton *)malloc(rows * columns * sizeof(tgbot_inlinekeyboardbutton));
	if (!keyboard->buttons) {
		free(keyboard);

		return NULL;
	}

	memset(keyboard->buttons, 0, rows * columns * sizeof(tgbot_inlinekeyboardbutton));

	return keyboard;
}

void tgbot_destroy_inlinekeyboard(tgbot_inlinekeyboard *keyboard) {
	free(keyboard->buttons);
	free(keyboard);
}

tgbot_rc tgbot_inlinekeyboard_button(tgbot_inlinekeyboard *keyboard, size_t row, size_t column, const char *text, const char *url, const char *callback_data) {
	tgbot_inlinekeyboardbutton *button = tgbot_inlinekeyboard_button_at(keyboard, row, column);

	strncpy(button->text, text, sizeof(button->text) - 1);
	strncpy(button->url, url, sizeof(button->url) - 1);
	strncpy(button->callback_data, callback_data, sizeof(button->callback_data) - 1);

	return TGBOT_OK;
}

tgbot_inlinekeyboardbutton *tgbot_inlinekeyboard_button_at(tgbot_inlinekeyboard *keyboard, size_t row, size_t column) {
	return &keyboard->buttons[row * keyboard->columns + column];
}
