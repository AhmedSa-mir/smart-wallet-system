
#ifndef CLIENT_HANDLER_H_
#define CLIENT_HANDLER_H_

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sstream>

#include "client_info.h"
#include "communication.h"

class ClientHandler {
public:
	ClientHandler();
	~ClientHandler();

	ClientInfo getClientInfo(unsigned long national_id);
	bool isValidId(std::string id);
	bool isValidAge(int age);

	bool createNewAccount(const ClientInfo& client_info);
	bool deposit(unsigned long long balance);
	bool withdraw(unsigned long long balance);
	unsigned long long getBalance();

	void sendResponse(Response response, int sockfd);
	std::string recvRequest(int sockfd);
	bool processRequest(std::string data, Response& response);
	bool serveRequests(int sockfd);
};

#endif