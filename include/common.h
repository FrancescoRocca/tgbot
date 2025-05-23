#ifndef TGBOT_COMMON_H
#define TGBOT_COMMON_H

#include <curl/curl.h>
#include <stdint.h>

/**
 * @brief A structure used to get curl response.
 */
struct memory_buffer {
	char *data;
	size_t size;
};

/**
 * @brief A structure to represent bot object.
 */
struct tgbot_t {
	char token[128]; /**< Bot token. */
	char api[512];	 /**< Bot API url. */
	int32_t offset;	 /**< Bot offset. */
	CURL *curl;		 /**< Curl object, used to send http requests. */
};
typedef struct tgbot_t tgbot;

/**
 * @brief A structure to represent Bot information got from getMe API.
 */
struct tgbot_me_t {
	char first_name[256]; /**< Bot's first name. */
	char username[32];	  /**< Bot's username. */
};
typedef struct tgbot_me_t tgbot_me;

/**
 * @brief A structure to represent Update object.
 */
struct tgbot_update_t {
	int64_t update_id;		   /**< Update id. */
	long message_id;		   /**< Message id. */
	int64_t chat_id;		   /**< Chat id. */
	char chat_first_name[256]; /**< Chat first name. */
	char chat_last_name[256];  /**< Chat last name. */
	char chat_username[32];	   /**< Chat username. */
	char chat_type[32];		   /**< Chat type (private/public). */
	int32_t date;			   /**< Date in unix timestamp. */
	char text[4096];		   /**< Message text. */
};
typedef struct tgbot_update_t tgbot_update;

/**
 * @brief A structure to represent CallbackQuery object.
 */
struct tgbot_cbquery_t {
	int64_t update_id;
	long message_id;
	int64_t chat_id;
	char chat_username[32];
	int32_t date;
	char text[4096];
	char chat_instance[128];
	char data[64]; /**> Callback data. */
};
typedef struct tgbot_cbquery_t tgbot_cbquery;

/**
 * @brief Callback function pointer.
 */
typedef void (*Callback)(tgbot *bot, tgbot_cbquery *query);

/**
 * @brief An enum to represent error codes.
 */
enum tgbot_rc_t {
	TGBOT_OK = 0,
	TGBOT_INIT_ERROR,
	TGBOT_REQUEST_ERROR,
	TGBOT_GETUPDATES_ERROR,
	TGBOT_GETME_ERROR,
	TGBOT_SENDMESSAGE_ERROR,
	TGBOT_EDITMESSAGETEXT_ERROR,
	TGBOT_SENDDICE_ERROR,
	TGBOT_TELEGRAM_OK_ERROR,
};
typedef enum tgbot_rc_t tgbot_rc;

enum tgbot_json_opt_type_t {
	tgbot_opt_int,
	tgbot_opt_int64,
	tgbot_opt_string,
	tgbot_opt_inlinekeyboard,
	tgbot_opt_null,
};
typedef enum tgbot_json_opt_type_t tgbot_json_opt_type;

struct tgbot_json_option_t {
	char key[32];
	void *value;
	tgbot_json_opt_type type;
};
typedef struct tgbot_json_option_t tgbot_json_option;

#endif
