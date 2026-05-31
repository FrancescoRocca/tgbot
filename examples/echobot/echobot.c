#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <tgbot/methods.h>
#include <tgbot/tgbot.h>

bool run = true;

void sighandler(int signum) {
	(void)signum;
	run = false;
}

void echo_message(tgbot_s *bot, tgbot_update_s *update) {
	tgbot_message message = {
		.chat_id = update->chat_id,
		.text = update->text,
		.parse_mode = "MARKDOWN",
		.reply_markup = NULL,
	};

	tgbot_send_message(bot, &message);
}

int main(void) {
	/* Retrieve bot's token */
	FILE *fp = fopen(".token", "r");
	if (fp == NULL) {
		fprintf(stderr, "Unable to retrieve bot token\n");
		return 1;
	}

	char token[256];
	fscanf(fp, "%s", token);
	fprintf(stdout, "Token: %s\n", token);
	fclose(fp);

	signal(SIGINT, sighandler);

	/* Initialize bot */
	tgbot_s *bot = tgbot_new(token);
	tgbot_update_s update;

	while (run) {
		if (tgbot_get_update(bot, &update, NULL) != 0) {
			continue;
		}
		if (strcmp(update.text, "/start") == 0) {
			/* Send dice if /start otherwise echo the message */
			tgbot_dice dice = {
				.chat_id = update.chat_id,
				.emoji = NULL,
			};

			tgbot_send_dice(bot, &dice);
		} else {
			echo_message(bot, &update);
		}
	}

	fprintf(stdout, "Closing...");
	tgbot_free(bot);

	return 0;
}
