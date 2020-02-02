
#ifndef CLIENT_HANDLER_H_
#define CLIENT_HANDLER_H_

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <mysql.h>

#include "client_info.h"
#include "defs.h"
#include "thread-safe_queue.h"

class ClientHandler {
public:
	ClientHandler(ThreadSafeQueue<Transaction>* loggingQueue);
	~ClientHandler();

	bool getClientInfo(unsigned long national_id, ClientInfo& client_info);
	bool isValidId(std::string id);
	bool isValidAge(int age);

	bool createNewAccount(const ClientInfo& client_info);
	bool deposit(unsigned long long balance);
	bool withdraw(unsigned long long balance);
	bool getBalance(unsigned long long& balance);

	void sendResponse(Response response, int sockfd);
	std::string recvRequest(int sockfd);
	bool processRequest(std::string data, Response& response);
	bool serveRequests(int sockfd);

private:
	ClientInfo client_info_;
	MYSQL *conn_;
	MYSQL_RES *res_;
	MYSQL_ROW row_;
	ThreadSafeQueue<Transaction>* loggingQueue_;
};

#endif