#include <stdio.h>
#include <stdlib.h>

#include <tgbot/tgbot.h>

tgbot_s *tgbot_new(const char *token) {
	if (!token || token[0] == '\0') {
		return NULL;
	}

	tgbot_s *bot = malloc(sizeof(tgbot_s));
	if (!bot) {
		return NULL;
	}

	int chars = snprintf(bot->api, sizeof(bot->api), "https://api.telegram.org/bot%s/", token);
	if (chars < 0 || (size_t)chars >= sizeof(bot->api)) {
		free(bot);
		return NULL;
	}

	bot->offset = 0;

	return bot;
}

void tgbot_free(tgbot_s *bot) {
	if (bot) {
		free(bot);
	}
}
