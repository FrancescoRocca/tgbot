#include "json.h"

json_object *tgbot_new_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup **keyboard, size_t rows, size_t columns) {
	json_object *reply_markup = json_object_new_object();
	json_object *inline_keyboard_array = json_object_new_array();

	for (size_t i = 0; i < rows; ++i) {
		json_object *row = json_object_new_array();
		for (size_t j = 0; j < columns; ++j) {
			if (strcmp(keyboard[i][j].text, "") == 0) {
				continue;
			}

			json_object *button = json_object_new_object();
			json_object_object_add(button, "text", json_object_new_string(keyboard[i][j].text));
			json_object_object_add(button, "url", json_object_new_string(keyboard[i][j].url));
			json_object_object_add(button, "callback_data", json_object_new_string(keyboard[i][j].callback_data));

			json_object_array_add(row, button);
		}
		json_object_array_add(inline_keyboard_array, row);
	}

	json_object_object_add(reply_markup, "inline_keyboard", inline_keyboard_array);

	return reply_markup;
}
