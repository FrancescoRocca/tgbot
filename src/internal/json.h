#ifndef TGBOT_INTERNAL_JSON_H
#define TGBOT_INTERNAL_JSON_H

#include "types.h"
#include <json-c/json.h>

typedef struct tgbot_option tgbot_option_s;

json_object *json_builder(tgbot_option_s *options, size_t optionslen);

json_object *json_ikb_new(tgbot_inlinekeyboard_s *keyboard);

#endif
