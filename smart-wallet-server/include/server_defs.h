
#ifndef SERVER_DEFS_H_
#define SERVER_DEFS_H_

#define MAX_AGE 200
#define MIN_AGE 18

#include <string>

typedef struct ClientInfo {
	unsigned long id;
	std::string name;
	unsigned long national_id;
	unsigned int age;
	unsigned long long balance;
	std::string gender;
} ClientInfo;

typedef struct DBParams {
	std::string ip;
	std::string user;
	std::string password;
	std::string db_name;
} DBParams;

typedef struct Transaction {
	std::string customer_name;
	unsigned long customer_id;
	REQUEST_TYPE type;
	unsigned long long amount;
	std::string date;
} Transaction;

#endif
