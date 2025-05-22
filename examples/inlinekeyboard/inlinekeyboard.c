#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tgbot.h>

#define WELCOME_MSG "Hi there! This bot is coded in C."

void parse_command(tgbot *bot, tgbot_update *update);
void sighandler(int signum);
void callback_parser(tgbot *bot, tgbot_cbquery *query);

bool run = true;
tgbot bot;
tgbot_inlinekeyboard *keyboard;

/* Callback handler function */
void callback_handler(tgbot *bot, tgbot_cbquery *query) {
	if (strcmp("test-callback", query->data) == 0) {
		/* Handle `test-callback` */
		tgbot_inlinekeyboard *home_keyboard = tgbot_new_inlinekeyboard(1, 1);
		/* Add buttons */
		tgbot_inlinekeyboard_button(home_keyboard, 0, 0, "Home", "", "home");
		tgbot_edit_message_text(bot, query->chat_id, query->message_id, "Callback called!", home_keyboard);
		tgbot_destroy_inlinekeyboard(home_keyboard);

		return;
	} else if (strcmp("home", query->data) == 0) {
		tgbot_edit_message_text(bot, query->chat_id, query->message_id, WELCOME_MSG, keyboard);

		return;
	}
}

void sighandler(int signum) {
	fprintf(stdout, "Closing...\n");
	run = false;
}

void parse_command(tgbot *bot, tgbot_update *update) {
	tgbot_rc ret;

	if (strcmp("/start", update->text) == 0) {
		ret = tgbot_send_message(bot, update->chat_id, WELCOME_MSG, "Markdown", keyboard);

		if (ret != TGBOT_OK) {
			fprintf(stderr, "Failed to send message\n");
		}

		return;
	}

	/* Echo the message */
	ret = tgbot_send_message(bot, update->chat_id, update->text, "Markdown", NULL);
	if (ret != TGBOT_OK) {
		fprintf(stderr, "Failed to send message\n");
	}
}

int main(void) {
	/* Find "your" way to free the resources */
	signal(SIGINT, sighandler);

	tgbot_rc ret;
	char token[256];

	FILE *fp = fopen(".token", "r");
	if (fp == NULL) {
		fprintf(stderr, "No .token file found!\n");
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%s", token);
	fprintf(stdout, "Token: %s\n", token);
	fclose(fp);

	/* Initialize the bot */
	ret = tgbot_init(&bot, token);
	if (ret != TGBOT_OK) {
		fprintf(stderr, "tgbot_init()\n");
		exit(1);
	}

	/* Calling tgbot_get_me() you can get bot's info */
	fprintf(stdout, "Running Telegram bot...\nPress Ctrl-C to close.\n");

	tgbot_update update;
	/* Allocate the new inline keyboard (remember to free!) */
	keyboard = tgbot_new_inlinekeyboard(1, 2);
	if (keyboard == NULL) {
		tgbot_destroy(&bot);
		return 1;
	}

	/* Populate the InlineKeyboardMarkup */
	tgbot_inlinekeyboard_button(keyboard, 0, 0, "Google", "https://google.com", "");
	tgbot_inlinekeyboard_button(keyboard, 0, 1, "Callback", "", "test-callback");
	/* If you want 3 buttons on 2 rows, for example on the first row 2 buttons and on the second only one */
	/* you have to put rows = 2, columns = 2 and pass an empty string to .text field */

	/* Main loop */
	while (run) {
		ret = tgbot_get_update(&bot, &update, callback_handler);
		if (ret != TGBOT_OK) {
			fprintf(stderr, "tgbot_get_updates()\n");
			continue;
		}

		if (update.message_id > 0) {
			parse_command(&bot, &update);
		}
	}

	tgbot_destroy_inlinekeyboard(keyboard);
	tgbot_destroy(&bot);

	return 0;
}
