#ifndef TGBOT_INTERNAL_PARSE_H
#define TGBOT_INTERNAL_PARSE_H

#include "types.h"
#include <json-c/json.h>

int tgbot_parse_message(tgbot_s *bot, tgbot_update_s *update, json_object *result);

int tgbot_parse_cbquery(tgbot_s *bot, tgbot_cbquery_s *query, json_object *result, Callback cbq_handler);

#endif
