#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "internal/common.h"
#include <tgbot/tgbot.h>

tgbot_s *tgbot_new(const char *token) {
	if (!token || token[0] == '\0') {
		return NULL;
	}

	tgbot_s *bot = malloc(sizeof(tgbot_s));
	if (!bot) {
		return NULL;
	}

	if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
		free(bot);
		return NULL;
	}

	int chars = snprintf(bot->api, sizeof(bot->api), "https://api.telegram.org/bot%s/", token);
	if (chars < 0 || (size_t)chars >= sizeof(bot->api)) {
		curl_global_cleanup();
		free(bot);
		return NULL;
	}

	chars = snprintf(bot->token, sizeof(bot->token), "%s", token);
	if (chars < 0 || (size_t)chars >= sizeof(bot->token)) {
		curl_global_cleanup();
		free(bot);
		return NULL;
	}

	bot->offset = 0;

	return bot;
}

void tgbot_free(tgbot_s *bot) {
	if (!bot) {
		return;
	}

	curl_global_cleanup();
	free(bot);
}
