#ifndef TGBOT_INTERNAL_REQUEST_H
#define TGBOT_INTERNAL_REQUEST_H

#include <stddef.h>
#include <tgbot/types.h>

/**
 * @brief A structure used to accumulate curl response data.
 */
struct memory_buffer {
	char *data;
	size_t size;
};

/**
 * @brief Perform an HTTP request to the Telegram Bot API.
 *
 * @param[in] url Full API endpoint URL.
 * @param[out] mb Pointer to a memory buffer for the response. NULL to discard.
 * @param[in] json Optional JSON body for POST. NULL for GET.
 *
 * @return 0 on success, -1 on failure.
 */
int tgbot_request(const char *url, struct memory_buffer **mb, const char *json);

#endif
