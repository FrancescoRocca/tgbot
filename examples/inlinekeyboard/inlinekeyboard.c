#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tgbot/methods.h>
#include <tgbot/tgbot.h>

#define WELCOME_MSG "Hi there! This bot is coded in C."

static volatile sig_atomic_t run = 1;
static tgbot_s *bot;
static tgbot_inlinekeyboard_s *keyboard;

static void sighandler(int signum) {
	(void)signum;
	run = 0;
}

static void handle_callback(tgbot_cbquery_s *query) {
	if (strcmp("test-callback", query->data) == 0) {
		tgbot_inlinekeyboard_s *home_keyboard = tgbot_inlinekb_new(1, 1);
		tgbot_inlinekb_button(home_keyboard, 0, 0, "Home", "", "home");

		tgbot_message edit = {
			.chat_id = query->chat_id,
			.message_id = query->message_id,
			.text = "Callback called!",
			.reply_markup = home_keyboard,
		};
		tgbot_edit_message_text(bot, &edit);

		tgbot_inlinekb_free(home_keyboard);
	} else if (strcmp("home", query->data) == 0) {
		tgbot_message edit = {
			.chat_id = query->chat_id,
			.message_id = query->message_id,
			.text = WELCOME_MSG,
			.reply_markup = keyboard,
		};
		tgbot_edit_message_text(bot, &edit);
	}
}

static void handle_message(tgbot_update_s *msg) {
	if (strcmp("/start", msg->text) == 0) {
		tgbot_message message = {
			.chat_id = msg->chat_id,
			.text = WELCOME_MSG,
			.parse_mode = "MARKDOWN",
			.reply_markup = keyboard,
		};
		tgbot_send_message(bot, &message);
	} else {
		tgbot_message echo = {
			.chat_id = msg->chat_id,
			.text = msg->text,
			.parse_mode = "MARKDOWN",
			.reply_markup = NULL,
		};
		tgbot_send_message(bot, &echo);
	}
}

int main(void) {
	signal(SIGINT, sighandler);

	char token[256];
	FILE *fp = fopen(".token", "r");
	if (!fp) {
		fprintf(stderr, "No .token file found!\n");
		return 1;
	}
	fscanf(fp, "%255s", token);
	fclose(fp);

	bot = tgbot_new(token);
	if (!bot) {
		return 1;
	}

	keyboard = tgbot_inlinekb_new(1, 2);
	if (!keyboard) {
		tgbot_free(bot);
		return 1;
	}

	tgbot_inlinekb_button(keyboard, 0, 0, "Google", "https://google.com", "");
	tgbot_inlinekb_button(keyboard, 0, 1, "Callback", "", "test-callback");

	tgbot_updates_t updates = {0};

	while (run) {
		if (tgbot_get_updates(bot, &updates) != 0) {
			continue;
		}

		for (size_t i = 0; i < updates.count; ++i) {
			switch (updates.items[i].type) {
				case MESSAGE:
					handle_message(&updates.items[i].message);
					break;
				case CALLBACK_QUERY:
					handle_callback(&updates.items[i].cbquery);
					break;
			}
		}

		tgbot_updates_free(&updates);
	}

	tgbot_inlinekb_free(keyboard);
	tgbot_free(bot);
	return 0;
}
