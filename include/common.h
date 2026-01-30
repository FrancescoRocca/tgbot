#ifndef TGBOT_COMMON_H
#define TGBOT_COMMON_H

#include <curl/curl.h>
#include <stdint.h>

#define TOKEN_SIZE 128
#define API_SIZE 512
#define URL_LEN 1024

#define FIRSTNAME_SIZE 256
#define USERNAME_SIZE 32

#define CHAT_LASTNAME_SIZE 256
#define CHAT_TYPE_SIZE 32
#define CHAT_TEXT_SIZE 4096

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
typedef struct tgbot {
	char token[TOKEN_SIZE]; /**< Bot token. */
	char api[API_SIZE];		/**< Bot API url. */
	int32_t offset;			/**< Bot offset. */
} tgbot_s;

/**
 * @brief A structure to represent Bot information got from getMe API.
 */
typedef struct tgbot_me {
	char first_name[FIRSTNAME_SIZE]; /**< Bot's first name. */
	char username[USERNAME_SIZE];	 /**< Bot's username. */
} tgbot_me_s;

/**
 * @brief A structure to represent Update object.
 */
typedef struct tgbot_update {
	int64_t update_id;						 /**< Update id. */
	long message_id;						 /**< Message id. */
	int64_t chat_id;						 /**< Chat id. */
	char chat_first_name[FIRSTNAME_SIZE];	 /**< Chat first name. */
	char chat_last_name[CHAT_LASTNAME_SIZE]; /**< Chat last name. */
	char chat_username[USERNAME_SIZE];		 /**< Chat username. */
	char chat_type[CHAT_TYPE_SIZE];			 /**< Chat type (private/public). */
	int32_t date;							 /**< Date in unix timestamp. */
	char text[CHAT_TEXT_SIZE];				 /**< Message text. */
} tgbot_update_s;

/**
 * @brief A structure to represent CallbackQuery object.
 */
typedef struct tgbot_cbquery {
	int64_t update_id;
	long message_id;
	int64_t chat_id;
	char chat_username[USERNAME_SIZE];
	int32_t date;
	char text[CHAT_TEXT_SIZE];
	char chat_instance[128];
	char data[64]; /**> Callback data. */
} tgbot_cbquery_s;

/**
 * @brief Callback function pointer.
 */
typedef void (*Callback)(tgbot_s *bot, tgbot_cbquery_s *query);

/**
 * @brief An enum to represent error codes.
 */
enum tgbot_rc {
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
typedef enum tgbot_rc tgbot_rc;

enum tgbot_opt_type {
	tgbot_opt_int,
	tgbot_opt_int64,
	tgbot_opt_string,
	tgbot_opt_inlinekeyboard,
	tgbot_opt_null,
};
typedef enum tgbot_opt_type tgbot_opt_type_e;

struct tgbot_option {
	char key[32];
	void *value;
	tgbot_opt_type_e type;
};
typedef struct tgbot_option tgbot_option_s;

#endif
