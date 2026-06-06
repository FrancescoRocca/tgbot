#include <stdio.h>
#include <string.h>

#include "internal/common.h"
#include "internal/json.h"
#include <tgbot/types.h>

json_object *json_builder(tgbot_option_s *options, size_t optionslen) {
	if (!options && optionslen > 0) {
		return NULL;
	}

	json_object *rjson = json_object_new_object();
	if (!rjson) {
		return NULL;
	}

	for (size_t i = 0; i < optionslen; ++i) {
		switch (options[i].type) {
			case tgbot_opt_int32: {
				if (!options[i].value) {
					break;
				}
				json_object_object_add(rjson, options[i].key, json_object_new_int(*((int32_t *)options[i].value)));
				break;
			}
			case tgbot_opt_string: {
				if (!options[i].value) {
					break;
				}
				json_object_object_add(rjson, options[i].key, json_object_new_string(options[i].value));
				break;
			}
			case tgbot_opt_int64: {
				if (!options[i].value) {
					break;
				}
				json_object_object_add(rjson, options[i].key, json_object_new_int64(*((int64_t *)options[i].value)));
				break;
			}
			case tgbot_opt_inlinekeyboard: {
				if (options[i].value != NULL) {
					json_object *reply_markup = json_ikb_new((tgbot_inlinekeyboard_s *)options[i].value);
					if (!reply_markup) {
						json_object_put(rjson);
						return NULL;
					}
					json_object_object_add(rjson, options[i].key, reply_markup);
				}
				break;
			}
			case tgbot_opt_null: {
				break;
			}
		}
	}

	return rjson;
}

json_object *json_ikb_new(tgbot_inlinekeyboard_s *keyboard) {
	if (!keyboard) {
		return NULL;
	}

	json_object *reply_markup = json_object_new_object();
	if (!reply_markup) {
		return NULL;
	}

	json_object *inline_keyboard_array = json_object_new_array();
	if (!inline_keyboard_array) {
		json_object_put(reply_markup);
		return NULL;
	}

	for (size_t i = 0; i < keyboard->rows; ++i) {
		json_object *row = json_object_new_array();
		if (!row) {
			json_object_put(inline_keyboard_array);
			json_object_put(reply_markup);
			return NULL;
		}

		for (size_t j = 0; j < keyboard->columns; ++j) {
			tgbot_inlinekeyboardbutton_s *kbbutton = tgbot_inlinekb_button_at(keyboard, i, j);
			if (!kbbutton || kbbutton->text[0] == '\0') {
				continue;
			}

			json_object *button = json_object_new_object();
			if (!button) {
				continue;
			}

			json_object_object_add(button, "text", json_object_new_string(kbbutton->text));
			if (kbbutton->url[0] != '\0') {
				json_object_object_add(button, "url", json_object_new_string(kbbutton->url));
			}
			if (kbbutton->callback_data[0] != '\0') {
				json_object_object_add(button, "callback_data", json_object_new_string(kbbutton->callback_data));
			}

			json_object_array_add(row, button);
		}
		json_object_array_add(inline_keyboard_array, row);
	}

	json_object_object_add(reply_markup, "inline_keyboard", inline_keyboard_array);

	return reply_markup;
}
