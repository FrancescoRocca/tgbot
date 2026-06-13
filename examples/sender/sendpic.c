#include <stdio.h>
#include <string.h>

#include <tgbot/methods.h>
#include <tgbot/tgbot.h>

#define START_MESSAGE "Send /photo to receive a nice landscape!"
#define PHOTO_PATH "my_photo.jpg"

int main(void) {
	FILE *tok = fopen(".token", "r");
	if (!tok) {
		return 1;
	}

	char token[512];
	fscanf(tok, "%s", token);
	fclose(tok);

	tgbot_s *bot = tgbot_new(token);
	if (!bot) {
		return 1;
	}

	tgbot_updates_t updates = {0};

	while (1) {
		if (tgbot_get_updates(bot, &updates) != 0) {
			continue;
		}

		for (size_t i = 0; i < updates.count; ++i) {
			if (updates.items[i].type != MESSAGE) {
				continue;
			}

			tgbot_update_s *msg = &updates.items[i].message;

			if (strcmp(msg->text, "/start") == 0) {
				tgbot_message message = {
					.chat_id = msg->chat_id,
					.text = START_MESSAGE,
					.parse_mode = "MARKDOWN",
					.reply_markup = NULL,
				};
				tgbot_send_message(bot, &message);
			} else if (strcmp(msg->text, "/photo") == 0) {
				tgbot_photo photo = {
					.chat_id = msg->chat_id,
					.path = PHOTO_PATH,
					.caption = "Mountains!",
				};
				tgbot_send_photo(bot, &photo);
			}
		}

		tgbot_updates_free(&updates);
	}

	return 0;
}
