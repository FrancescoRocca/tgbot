#ifndef TGBOT_H
#define TGBOT_H

#include <curl/curl.h>
#include <json-c/json.h>
#include <stdint.h>

/**
 * A structure used to get curl response
 */
struct memory_buffer {
	char *data;
	size_t size;
};

/**
 * A structure to represent bot object
 */
struct tgbot_t {
	char token[128]; /**< Bot token */
	char api[512];	 /**< Bot API url */
	int32_t offset;	 /**< Bot offset */
	CURL *curl;		 /**< Curl object, used to send http requests */
};
typedef struct tgbot_t tgbot;

/**
 * A structure to represent Bot information got from getMe API
 */
struct tgbot_me_t {
	char first_name[256];
	char username[32];
};
typedef struct tgbot_me_t tgbot_me;

/* TODO: implement variable length string */
/**
 * A structure to represent Update object
 */
struct tgbot_update_t {
	int64_t update_id;		   /**< Update id */
	long message_id;		   /**< Message id */
	int64_t chat_id;		   /**< Chat id */
	char chat_first_name[256]; /**< Chat first name */
	char chat_last_name[256];  /**< Chat last name */
	char chat_username[32];	   /**< Chat username */
	char chat_type[32];		   /**< Chat type (private/public) */
	int32_t date;			   /**< Date in unix timestamp */
	char text[4096];		   /**< Message text */
};
typedef struct tgbot_update_t tgbot_update;

/**
 * A structure to represent CallbackQuery object
 */
struct tgbot_cbquery_t {
	int64_t update_id;
	long message_id;
	int64_t chat_id;
	char chat_username[32];
	int32_t date;
	char text[4096];
	char chat_instance[128];
	char data[64]; /**> Callback data */
};
typedef struct tgbot_cbquery_t tgbot_cbquery;

/**
 * Callback function pointer
 */
typedef void (*Callback)(tgbot *bot, tgbot_cbquery *query);

/**
 * A structure to represent error codes
 */
enum tgbot_rc {
	TGBOT_OK = 0,
	TGBOT_INIT_ERROR,
	TGBOT_REQUEST_ERROR,
	TGBOT_GETUPDATES_ERROR,
	TGBOT_GETME_ERROR,
	TGBOT_SENDMESSAGE_ERROR,
	TGBOT_TELEGRAM_OK_ERROR,
};
typedef enum tgbot_rc tgbot_rc;

/**
 * Initializes the Bot object
 * @param[out] bot The Bot object
 * @param[in] token The Bot token (obtained from @BotFather)
 */
tgbot_rc tgbot_init(tgbot *bot, char *token);

/**
 * Cleans the memory
 * @param bot The Bot object
 */
void tgbot_destroy(tgbot *bot);

/* Types definitions */

/**
 * A structure to represent InlineKeyboardMarkup
 */
struct tgbot_inlinekeyboardmarkup_t {
	char text[200];			/**< If this field is empty the button will be skipped */
	char url[200];			/**< (Optional) URL of the button */
	char callback_data[64]; /**< Callback data */
};
typedef struct tgbot_inlinekeyboardmarkup_t tgbot_inlinekeyboardmarkup;

/* Types methods */
tgbot_rc
tgbot_allocate_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup ***keyboard, size_t rows, size_t columns);
tgbot_rc tgbot_deallocate_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup **keyboard, size_t rows);
json_object *tgbot_new_inlinekeyboardmarkup(tgbot_inlinekeyboardmarkup **keyboard, size_t rows, size_t columns);

/* Methods */
tgbot_rc tgbot_get_update(tgbot *bot, tgbot_update *update, Callback cbq_handler);
tgbot_rc tgbot_parse_message(tgbot *bot, tgbot_update *update, json_object *result);
tgbot_rc tgbot_parse_cbquery(tgbot *bot, tgbot_cbquery *query, json_object *result, Callback query_handler);
tgbot_rc tgbot_get_me(tgbot *bot, tgbot_me *me);
tgbot_rc tgbot_send_message(tgbot *bot, int64_t chat_id, char *text, char *parse_mode, tgbot_inlinekeyboardmarkup **reply_markup, size_t rows, size_t columns);

/* Updating Methods */
tgbot_rc tgbot_edit_message_text(tgbot *bot, int64_t chat_id, long message_id, char *text, tgbot_inlinekeyboardmarkup **keyboard, size_t rows, size_t columns);

/* Request */
size_t write_callback(void *ptr, size_t size, size_t nmemb, char **userdata);
tgbot_rc tgbot_request(tgbot *bot, char *url, struct memory_buffer **mb, json_object *json);

#endif // TGBOT_H
