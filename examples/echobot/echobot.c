#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <tgbot.h>

bool run = true;

void sighandler(int signum) {
	run = false;
}

void echo_message(tgbot *bot, tgbot_update *update) {
	tgbot_send_message(bot, update->chat_id, update->text, "MARKDOWN", NULL);
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
	tgbot bot;
	tgbot_init(&bot, token);
	tgbot_update update;

	while (run) {
		tgbot_get_update(&bot, &update, NULL);
		echo_message(&bot, &update);
	}

	fprintf(stdout, "Closing...");
	tgbot_destroy(&bot);

	return 0;
}