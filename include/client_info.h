
#ifndef CLIENT_INFO_H_
#define CLIENT_INFO_H_

#include <string>

typedef struct ClientInfo {
	std::string name;
	unsigned long national_id;
	unsigned int age;
	unsigned long long balance;
} ClientInfo;

#endif