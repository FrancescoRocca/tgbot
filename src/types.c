#include <stdlib.h>
#include <string.h>

#include "types.h"

tgbot_inlinekeyboard_s *tgbot_inlinekb_new(size_t rows, size_t columns) {
	tgbot_inlinekeyboard_s *keyboard = (tgbot_inlinekeyboard_s *)malloc(sizeof(tgbot_inlinekeyboard_s));
	if (!keyboard) {
		return NULL;
	}

	keyboard->rows = rows;
	keyboard->columns = columns;
	keyboard->buttons = (tgbot_inlinekeyboardbutton_s *)malloc(rows * columns * sizeof(tgbot_inlinekeyboardbutton_s));
	if (!keyboard->buttons) {
		free(keyboard);

		return NULL;
	}

	memset(keyboard->buttons, 0, rows * columns * sizeof(tgbot_inlinekeyboardbutton_s));

	return keyboard;
}

void tgbot_inlinekb_free(tgbot_inlinekeyboard_s *keyboard) {
	free(keyboard->buttons);
	free(keyboard);
}

tgbot_inlinekeyboardbutton_s *tgbot_inlinekb_button_at(tgbot_inlinekeyboard_s *keyboard, size_t row, size_t column) {
	return &keyboard->buttons[row * keyboard->columns + column];
}

tgbot_rc tgbot_inlinekb_button(tgbot_inlinekeyboard_s *keyboard, size_t row, size_t column, const char *text,
							   const char *url, const char *callback_data) {
	tgbot_inlinekeyboardbutton_s *button = tgbot_inlinekb_button_at(keyboard, row, column);

	strncpy(button->text, text, sizeof(button->text) - 1);
	strncpy(button->url, url, sizeof(button->url) - 1);
	strncpy(button->callback_data, callback_data, sizeof(button->callback_data) - 1);

	return TGBOT_OK;
}
