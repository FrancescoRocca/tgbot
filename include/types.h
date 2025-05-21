#ifndef TGBOT_TYPES_H
#define TGBOT_TYPES_H

#include "common.h"

/**
 * A structure to represent InlineKeyboardMarkup
 */
struct tgbot_inlinekeyboardmarkup_t {
	char text[200];			/**< If this field is empty the button will be skipped */
	char url[200];			/**< (Optional) URL of the button */
	char callback_data[64]; /**< Callback data */
};
typedef struct tgbot_inlinekeyboardmarkup_t tgbot_inlinekeyboardmarkup;

tgbot_rc
tgbot_allocate_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup ***keyboard, size_t rows, size_t columns);
tgbot_rc tgbot_deallocate_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup **keyboard, size_t rows);

#endif
