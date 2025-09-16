#include <string.h>

#include "json.h"

json_object *json_builder(tgbot_json_option *options, size_t optionslen) {
	json_object *rjson = json_object_new_object();

	for (size_t i = 0; i < optionslen; ++i) {
		if (options[i].type == tgbot_opt_int) {
			json_object_object_add(rjson, options[i].key, json_object_new_int(*((int32_t *)options[i].value)));
		} else if (options[i].type == tgbot_opt_string) {
			if (!options[i].value) {
				continue;
			}
			json_object_object_add(rjson, options[i].key, json_object_new_string((char *)options[i].value));
		} else if (options[i].type == tgbot_opt_int64) {
			json_object_object_add(rjson, options[i].key, json_object_new_int64(*((int64_t *)options[i].value)));
		} else if (options[i].type == tgbot_opt_inlinekeyboard) {
			if (options[i].value != NULL) {
				json_object *reply_markup = json_ikb_new((tgbot_inlinekeyboard_s *)options[i].value);
				json_object_object_add(rjson, "reply_markup", reply_markup);
			}
		} else if (options[i].type == tgbot_opt_null) {
			continue;
		}
	}

	return rjson;
}

json_object *json_ikb_new(tgbot_inlinekeyboard_s *keyboard) {
	json_object *reply_markup = json_object_new_object();
	json_object *inline_keyboard_array = json_object_new_array();

	for (size_t i = 0; i < keyboard->rows; ++i) {
		json_object *row = json_object_new_array();
		for (size_t j = 0; j < keyboard->columns; ++j) {
			tgbot_inlinekeyboardbutton_s *kbbutton = tgbot_inlinekb_button_at(keyboard, i, j);
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
