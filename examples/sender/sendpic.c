#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tgbot/methods.h>
#include <tgbot/tgbot.h>
#include <tgbot/types.h>

#define START_MESSAGE "Send /photo to receive a nice landscape!"
#define PHOTO_PATH "my_photo.jpg"

int main(void) {
	FILE *tok = fopen(".token", "r");
	if (!tok) {
		exit(EXIT_FAILURE);
	}

	char token[512];
	fscanf(tok, "%s", token);
	fprintf(stdout, "Token: %s\n", token);

	tgbot_s *bot = tgbot_new(token);
	if (!bot) {
		return -1;
	}

	tgbot_update_s update;
	while (1) {
		if (tgbot_get_update(bot, &update, NULL) != 0) {
			continue;
		}

		if (!strcmp(update.text, "/start")) {
			tgbot_message message = {
				.chat_id = update.chat_id,
				.text = START_MESSAGE,
				.parse_mode = "MARKDOWN",
				.reply_markup = NULL,
			};
			tgbot_send_message(bot, &message);
		} else if (!strcmp(update.text, "/photo")) {
			tgbot_photo photo = {
				.chat_id = update.chat_id,
				.path = PHOTO_PATH,
				.caption = "Mountains!",
			};
			tgbot_send_photo(bot, &photo);
		}
	}

	return 0;
}
