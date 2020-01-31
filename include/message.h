#include <stdint.h>

#define MSG_MAX_SIZE 300

enum MSG_STATUS {SUCCESS, FAIL};

typedef struct Message {
	char data[MSG_MAX_SIZE];
	uint32_t size;
	MSG_STATUS status;
} Message;