#ifndef TGBOT_PARSE_H
#define TGBOT_PARSE_H

#include "common.h"
#include <json-c/json.h>

tgbot_rc tgbot_parse_message(tgbot_s *bot, tgbot_update_s *update, json_object *result);

tgbot_rc tgbot_parse_cbquery(tgbot_s *bot, tgbot_cbquery_s *query, json_object *result, Callback cbq_handler);

#endif
