#ifndef TGBOT_JSON_H
#define TGBOT_JSON_H

#include "types.h"
#include <json-c/json.h>

json_object *json_builder(tgbot_json_option *options, size_t optionslen);

json_object *json_ikb_new(tgbot_inlinekeyboard_s *keyboard);

#endif
