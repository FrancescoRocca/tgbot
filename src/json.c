#include <stdlib.h>
#include <string.h>
#include <yyjson.h>

#include "internal/json.h"

static yyjson_mut_val *json_ikb_new(yyjson_mut_doc *doc, tgbot_inlinekeyboard_s *keyboard) {
	if (!keyboard || !doc) {
		return NULL;
	}

	yyjson_mut_val *reply_markup = yyjson_mut_obj(doc);
	yyjson_mut_val *inline_keyboard = yyjson_mut_arr(doc);
	if (!reply_markup || !inline_keyboard) {
		return NULL;
	}

	for (size_t i = 0; i < keyboard->rows; ++i) {
		yyjson_mut_val *row = yyjson_mut_arr(doc);
		if (!row) {
			return NULL;
		}

		for (size_t j = 0; j < keyboard->columns; ++j) {
			tgbot_inlinekeyboardbutton_s *btn = tgbot_inlinekb_button_at(keyboard, i, j);
			if (!btn || btn->text[0] == '\0') {
				continue;
			}

			yyjson_mut_val *button = yyjson_mut_obj(doc);
			if (!button) {
				continue;
			}

			yyjson_mut_obj_add_str(doc, button, "text", btn->text);

			if (btn->url[0] != '\0') {
				yyjson_mut_obj_add_str(doc, button, "url", btn->url);
			}

			if (btn->callback_data[0] != '\0') {
				yyjson_mut_obj_add_str(doc, button, "callback_data", btn->callback_data);
			}

			yyjson_mut_arr_append(row, button);
		}

		yyjson_mut_arr_append(inline_keyboard, row);
	}

	yyjson_mut_obj_add_val(doc, reply_markup, "inline_keyboard", inline_keyboard);

	return reply_markup;
}

char *json_builder(tgbot_option_s *options, size_t optionslen) {
	if (!options && optionslen > 0) {
		return NULL;
	}

	yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
	if (!doc) {
		return NULL;
	}

	yyjson_mut_val *root = yyjson_mut_obj(doc);
	if (!root) {
		yyjson_mut_doc_free(doc);
		return NULL;
	}
	yyjson_mut_doc_set_root(doc, root);

	for (size_t i = 0; i < optionslen; ++i) {
		if (!options[i].value) {
			continue;
		}

		switch (options[i].type) {
			case tgbot_opt_int32:
				yyjson_mut_obj_add_int(doc, root, options[i].key, *((int32_t *)options[i].value));
				break;
			case tgbot_opt_int64:
				yyjson_mut_obj_add_sint(doc, root, options[i].key, *((int64_t *)options[i].value));
				break;
			case tgbot_opt_string:
				yyjson_mut_obj_add_str(doc, root, options[i].key, (const char *)options[i].value);
				break;
			case tgbot_opt_inlinekeyboard: {
				yyjson_mut_val *rm = json_ikb_new(doc, (tgbot_inlinekeyboard_s *)options[i].value);
				if (!rm) {
					yyjson_mut_doc_free(doc);
					return NULL;
				}
				yyjson_mut_obj_add_val(doc, root, options[i].key, rm);
				break;
			}
		}
	}

	char *json_str = yyjson_mut_write(doc, 0, NULL);
	yyjson_mut_doc_free(doc);

	return json_str;
}
