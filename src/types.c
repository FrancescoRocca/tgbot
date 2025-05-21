#include "types.h"

tgbot_rc tgbot_allocate_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup ***keyboard, size_t rows, size_t columns) {
	*keyboard = (tgbot_inlinekeyboardmarkup **)malloc(rows * sizeof(tgbot_inlinekeyboardmarkup *));
	for (size_t i = 0; i < rows; ++i) {
		(*keyboard)[i] = (tgbot_inlinekeyboardmarkup *)malloc(columns * sizeof(tgbot_inlinekeyboardmarkup));
	}

	return TGBOT_OK;
}

tgbot_rc tgbot_deallocate_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup **keyboard, size_t rows) {
	for (size_t i = 0; i < rows; ++i) {
		free(keyboard[i]);
	}

	free(keyboard);

	return TGBOT_OK;
}
