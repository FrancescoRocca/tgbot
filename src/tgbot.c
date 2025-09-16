#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "tgbot.h"

tgbot_s *tgbot_new(char *token) {
	tgbot_s *bot = malloc(sizeof(tgbot_s));
	if (!bot) {
		return NULL;
	}

	curl_global_init(CURL_GLOBAL_DEFAULT);

	snprintf(bot->api, sizeof(bot->api), "https://api.telegram.org/bot%s/", token);
	snprintf(bot->token, sizeof(bot->token), "%s", token);
	bot->offset = 0;

	return bot;
}

void tgbot_free(tgbot_s *bot) {
	curl_global_cleanup();
	free(bot);
}
