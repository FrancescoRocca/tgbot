#include <string.h>

#include "json.h"

json_object *tgbot_new_inlinekeyboardmarkup(tgbot_inlinekeyboard *keyboard) {
	json_object *reply_markup = json_object_new_object();
	json_object *inline_keyboard_array = json_object_new_array();

	for (size_t i = 0; i < keyboard->rows; ++i) {
		json_object *row = json_object_new_array();
		for (size_t j = 0; j < keyboard->columns; ++j) {
			tgbot_inlinekeyboardbutton *kbbutton = tgbot_inlinekeyboard_button_at(keyboard, i, j);
			if (strcmp(kbbutton->text, "") == 0) {
				continue;
			}

			json_object *button = json_object_new_object();
			json_object_object_add(button, "text", json_object_new_string(kbbutton->text));
			json_object_object_add(button, "url", json_object_new_string(kbbutton->url));
			json_object_object_add(button, "callback_data", json_object_new_string(kbbutton->callback_data));

			json_object_array_add(row, button);
		}
		json_object_array_add(inline_keyboard_array, row);
	}

	json_object_object_add(reply_markup, "inline_keyboard", inline_keyboard_array);

	return reply_markup;
}
