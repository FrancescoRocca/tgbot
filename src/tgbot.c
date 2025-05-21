#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "tgbot.h"

tgbot_rc tgbot_init(tgbot *bot, char *token) {
	curl_global_init(CURL_GLOBAL_DEFAULT);

	snprintf(bot->api, sizeof(bot->api), "https://api.telegram.org/bot%s/", token);
	snprintf(bot->token, sizeof(bot->token), "%s", token);
	bot->offset = 0;
	bot->curl = curl_easy_init();

	if (!bot->curl) {
		return TGBOT_INIT_ERROR;
	}

	return TGBOT_OK;
}

void tgbot_destroy(tgbot *bot) {
	if (bot->curl) {
		curl_easy_cleanup(bot->curl);
	}
	curl_global_cleanup();
}
