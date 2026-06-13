#include <signal.h>
#include <stdio.h>
#include <string.h>

#include <tgbot/methods.h>
#include <tgbot/tgbot.h>

static volatile sig_atomic_t run = 1;

static void sighandler(int signum) {
	(void)signum;
	run = 0;
}

static void handle_message(tgbot_s *bot, tgbot_update_s *msg) {
	if (strcmp(msg->text, "/start") == 0) {
		tgbot_dice dice = {
			.chat_id = msg->chat_id,
			.emoji = NULL,
		};
		tgbot_send_dice(bot, &dice);
	} else {
		tgbot_message reply = {
			.chat_id = msg->chat_id,
			.text = msg->text,
			.parse_mode = "MARKDOWN",
			.reply_markup = NULL,
		};
		tgbot_send_message(bot, &reply);
	}
}

int main(void) {
	FILE *fp = fopen(".token", "r");
	if (!fp) {
		fprintf(stderr, "Unable to retrieve bot token\n");
		return 1;
	}

	char token[256];
	fscanf(fp, "%255s", token);
	fclose(fp);

	signal(SIGINT, sighandler);

	tgbot_s *bot = tgbot_new(token);
	tgbot_updates_t updates = {0};

	while (run) {
		if (tgbot_get_updates(bot, &updates) != 0) {
			continue;
		}

		for (size_t i = 0; i < updates.count; ++i) {
			if (updates.items[i].type == MESSAGE) {
				handle_message(bot, &updates.items[i].message);
			}
		}

		tgbot_updates_free(&updates);
	}

	tgbot_free(bot);
	return 0;
}
