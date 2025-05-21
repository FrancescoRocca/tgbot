#ifndef TGBOT_TYPES_H
#define TGBOT_TYPES_H

#include "common.h"

/**
 * A structure to represent keyboard's buttons
 */
struct tgbot_inlinekeyboardbutton_t {
	char text[200];			/**< If this field is empty the button will be skipped */
	char url[200];			/**< (Optional) URL of the button */
	char callback_data[64]; /**< Callback data */
};
typedef struct tgbot_inlinekeyboardbutton_t tgbot_inlinekeyboardbutton;

struct tgbot_inlinekeyboard_t {
	size_t rows;
	size_t columns;
	struct tgbot_inlinekeyboardbutton_t *buttons;
};
typedef struct tgbot_inlinekeyboard_t tgbot_inlinekeyboard;

tgbot_inlinekeyboard *tgbot_new_inlinekeyboard(size_t rows, size_t columns);
tgbot_rc tgbot_inlinekeyboard_button(tgbot_inlinekeyboard *keyboard, size_t row, size_t column, const char *text, const char *url, const char *callback_data);
tgbot_inlinekeyboardbutton *tgbot_inlinekeyboard_button_at(tgbot_inlinekeyboard *keyboard, size_t row, size_t column);
void tgbot_destroy_inlinekeyboard(tgbot_inlinekeyboard *keyboard);

#endif
