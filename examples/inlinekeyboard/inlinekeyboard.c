#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tgbot/methods.h>
#include <tgbot/tgbot.h>
#include <tgbot/types.h>

#define WELCOME_MSG "Hi there! This bot is coded in C."

void parse_command(const tgbot_s *bot, const tgbot_update_s *update);
void sighandler(int signum);

bool run = true;
tgbot_s *bot;
tgbot_inlinekeyboard_s *keyboard;

/* Callback handler function */
void callback_handler(tgbot_s *bot, tgbot_cbquery_s *query) {
	if (strcmp("test-callback", query->data) == 0) {
		/* Handle `test-callback` */
		tgbot_inlinekeyboard_s *home_keyboard = tgbot_inlinekb_new(1, 1);
		/* Add buttons */
		tgbot_inlinekb_button(home_keyboard, 0, 0, "Home", "", "home");

		tgbot_message edit = {
			.chat_id = query->chat_id,
			.message_id = query->message_id,
			.text = "Callback called!",
			.reply_markup = home_keyboard,
		};
		tgbot_edit_message_text(bot, &edit);

		tgbot_inlinekb_free(home_keyboard);

		return;
	} else if (strcmp("home", query->data) == 0) {
		tgbot_message edit = {
			.chat_id = query->chat_id,
			.message_id = query->message_id,
			.text = WELCOME_MSG,
			.reply_markup = keyboard,
		};
		tgbot_edit_message_text(bot, &edit);

		return;
	}
}

void sighandler(int signum) {
	(void)signum;
	fprintf(stdout, "Closing...\n");
	run = false;
}

void parse_command(const tgbot_s *bot, const tgbot_update_s *update) {
	int ret;

	if (strcmp("/start", update->text) == 0) {
		tgbot_message message = {
			.chat_id = update->chat_id,
			.text = WELCOME_MSG,
			.parse_mode = "MARKDOWN",
			.reply_markup = keyboard,
		};
		ret = tgbot_send_message(bot, &message);

		if (ret != 0) {
			fprintf(stderr, "Failed to send message\n");
		}

		return;
	}

	/* Echo the message */
	tgbot_message echo = {
		.chat_id = update->chat_id,
		.text = update->text,
		.parse_mode = "MARKDOWN",
		.reply_markup = NULL,
	};
	ret = tgbot_send_message(bot, &echo);
	if (ret != 0) {
		fprintf(stderr, "Failed to send message\n");
	}
}

int main(void) {
	/* Find "your" way to free the resources */
	signal(SIGINT, sighandler);

	char token[256];

	FILE *fp = fopen(".token", "r");
	if (fp == NULL) {
		fprintf(stderr, "No .token file found!\n");
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%255s", token);
	fprintf(stdout, "Token: %s\n", token);
	fclose(fp);

	/* Initialize the bot */
	bot = tgbot_new(token);
	if (!bot) {
		fprintf(stderr, "tgbot_init()\n");
		exit(1);
	}

	/* Calling tgbot_get_me() you can get bot's info */
	fprintf(stdout, "Running Telegram bot...\nPress Ctrl-C to close.\n");

	tgbot_update_s update;
	/* Allocate the new inline keyboard (remember to free!) */
	keyboard = tgbot_inlinekb_new(1, 2);
	if (keyboard == NULL) {
		tgbot_free(bot);
		return 1;
	}

	/* Populate the InlineKeyboardMarkup */
	tgbot_inlinekb_button(keyboard, 0, 0, "Google", "https://google.com", "");
	tgbot_inlinekb_button(keyboard, 0, 1, "Callback", "", "test-callback");
	/* If you want 3 buttons on 2 rows, for example on the first row 2 buttons and on the second only one */
	/* you have to put rows = 2, columns = 2 and pass an empty string to .text field */

	/* Main loop */
	while (run) {
		int ret = tgbot_get_update(bot, &update, callback_handler);
		if (ret != 0) {
			continue;
		}

		if (update.message_id > 0) {
			parse_command(bot, &update);
		}
	}

	tgbot_inlinekb_free(keyboard);
	tgbot_free(bot);

	return 0;
}
