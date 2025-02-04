#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "tgbot.h"

tgbot bot;
size_t rows = 1, columns = 2;
tgbot_inlinekeyboardmarkup **keyboard;

/* Callback handler function */
void callback_handler(tgbot *bot, tgbot_cbquery *query) {
	if (strcmp("test-callback", query->data) == 0) {
		/* Handle `test-callback` */

		tgbot_inlinekeyboardmarkup **home_keyboard;
		tgbot_allocate_inlinekeyboardmarkup(&home_keyboard, 1, 1);
		strncpy(home_keyboard[0][0].text, "Home", sizeof(home_keyboard[0][0].text));
		strncpy(home_keyboard[0][0].url, "", sizeof(home_keyboard[0][0].url));
		strncpy(home_keyboard[0][0].callback_data, "home", sizeof(home_keyboard[0][0].callback_data));

		tgbot_edit_message_text(bot, query->chat_id, query->message_id, "Callback called!", home_keyboard, 1, 1);
		tgbot_deallocate_inlinekeyboardmarkup(home_keyboard, 1);

		return;
	} else if (strcmp("home", query->data) == 0) {
		tgbot_edit_message_text(bot, query->chat_id, query->message_id, WELCOME_MSG, keyboard, rows, columns);

		return;
	}
}

void sighandler(int signum) {
	fprintf(stdout, "Cleaning resources...\n");
	tgbot_destroy(&bot);
	tgbot_deallocate_inlinekeyboardmarkup(keyboard, rows);

	exit(0);
}

void parse_command(tgbot *bot, tgbot_update *update) {
	tgbot_rc ret;

	if (strcmp("/start", update->text) == 0) {
		/* Example of an InlineKeyboardMarkup */
		/* Check global variables */
		tgbot_allocate_inlinekeyboardmarkup(&keyboard, rows, columns);

		/* Populate the InlineKeyboardMarkup */
		strncpy(keyboard[0][0].text, "Google", sizeof(keyboard[1][0].text));
		strncpy(keyboard[0][0].url, "https://google.com", sizeof(keyboard[1][0].url));
		strncpy(keyboard[0][0].callback_data, "", sizeof(keyboard[1][0].callback_data));

		strncpy(keyboard[0][1].text, "Callback", sizeof(keyboard[0][1].text));
		strncpy(keyboard[0][1].url, "", sizeof(keyboard[0][1].url));
		strncpy(keyboard[0][1].callback_data, "test-callback", sizeof(keyboard[0][1].callback_data));

		/* If you want 3 buttons on 2 rows, for example on the first row 2 buttons and on the second only one */
		/* you have to put rows = 2, columns = 2 and pass an empty string to .text field */

		ret = tgbot_send_message(bot, update->chat_id, WELCOME_MSG, "Markdown", keyboard, rows, columns);

		/* Deallocate the keyboard */
		/* See sighandler() */

		if (ret != TGBOT_OK) {
			fprintf(stderr, "Failed to send message\n");
		}

		return;
	}

	/* Echo the message */
	ret = tgbot_send_message(bot, update->chat_id, update->text, "Markdown", NULL, 0, 0);
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

	/* Main loop */
	for (;;) {
		ret = tgbot_get_update(&bot, &update, callback_handler);
		if (ret != TGBOT_OK) {
			fprintf(stderr, "tgbot_get_updates()\n");
			continue;
		}

		if (update.message_id > 0) {
			parse_command(&bot, &update);
		}
	}
}
