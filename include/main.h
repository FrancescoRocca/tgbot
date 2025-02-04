#ifndef MAIN_H
#define MAIN_H

#include "tgbot.h"

#define WELCOME_MSG "Hi there! This bot is coded in C."

void parse_command(tgbot *bot, tgbot_update *update);
void sighandler(int signum);
void callback_parser(tgbot *bot, tgbot_cbquery *query);

#endif // MAIN_H
