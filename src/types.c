#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tgbot/types.h>

tgbot_inlinekeyboard_s *tgbot_inlinekb_new(size_t rows, size_t columns) {
	if (rows == 0 || columns == 0 || rows > SIZE_MAX / columns) {
		return NULL;
	}

	tgbot_inlinekeyboard_s *keyboard = malloc(sizeof(tgbot_inlinekeyboard_s));
	if (!keyboard) {
		return NULL;
	}

	keyboard->rows = rows;
	keyboard->columns = columns;

	size_t count = rows * columns;
	keyboard->buttons = malloc(count * sizeof(tgbot_inlinekeyboardbutton_s));
	if (!keyboard->buttons) {
		free(keyboard);
		return NULL;
	}

	memset(keyboard->buttons, 0, count * sizeof(tgbot_inlinekeyboardbutton_s));

	return keyboard;
}

void tgbot_inlinekb_free(tgbot_inlinekeyboard_s *keyboard) {
	if (!keyboard) {
		return;
	}

	free(keyboard->buttons);
	free(keyboard);
}

tgbot_inlinekeyboardbutton_s *tgbot_inlinekb_button_at(tgbot_inlinekeyboard_s *keyboard, size_t row, size_t column) {
	if (!keyboard || row >= keyboard->rows || column >= keyboard->columns) {
		return NULL;
	}

	return &keyboard->buttons[row * keyboard->columns + column];
}

int tgbot_inlinekb_button(tgbot_inlinekeyboard_s *keyboard, size_t row, size_t column, const char *text,
						  const char *url, const char *callback_data) {
	tgbot_inlinekeyboardbutton_s *button = tgbot_inlinekb_button_at(keyboard, row, column);
	if (!button) {
		return -1;
	}

	snprintf(button->text, sizeof(button->text), "%s", text ? text : "");
	snprintf(button->url, sizeof(button->url), "%s", url ? url : "");
	snprintf(button->callback_data, sizeof(button->callback_data), "%s", callback_data ? callback_data : "");

	return 0;
}
