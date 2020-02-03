
#ifndef DEFS_H_
#define DEFS_H_

#include <stdint.h>

#define MSG_MAX_SIZE 3000

enum RESPONSE_STATUS {SUCCESS, FAIL, INVALID_ID, INVALID_AGE, INVALID_AMOUNT, NOT_ENOUGH_MONEY};
enum REQUEST_TYPE {NONE, LOGIN, REGISTER, DEPOSIT, WITHDRAW, CHECK_BALANCE, UNDO, REDO, BYE};


typedef struct Request {
	char data[MSG_MAX_SIZE];
	uint32_t size;
	REQUEST_TYPE type;
} Request;

typedef struct Response {
	char data[MSG_MAX_SIZE];
	uint32_t size;
	RESPONSE_STATUS status;
} Response;

typedef struct Transaction {
	std::string customer_name;
	unsigned long customer_id;
	REQUEST_TYPE type;
	unsigned long long amount;
	std::string date;
} Transaction;

#endif
