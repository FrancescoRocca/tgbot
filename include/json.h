#ifndef TGBOT_JSON_H
#define TGBOT_JSON_H

#include "types.h"
#include <json-c/json.h>

json_object *tgbot_new_inlinekeyboardmarkup(tgbot_inlinekeyboard *keyboard);

#endif
