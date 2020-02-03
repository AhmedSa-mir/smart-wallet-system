
#ifndef CLIENT_HANDLER_H_
#define CLIENT_HANDLER_H_

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <stack>
#include <vector>
#include <mysql.h>

#include "client_info.h"
#include "defs.h"
#include "thread-safe_queue.h"

class ClientHandler {
public:
	ClientHandler(ThreadSafeQueue<Transaction>* loggingQueue);
	~ClientHandler();

	int getClientInfo(unsigned long national_id, ClientInfo& client_info);
	bool isValidId(std::string id);
	bool isValidAge(int age);

	bool createNewAccount(const ClientInfo& client_info);
	bool deposit(unsigned long long balance);
	bool withdraw(unsigned long long balance);
	bool getBalance(unsigned long long& balance);
	bool getStats(std::vector<std::string>& transaction_dates,
							 std::vector<unsigned long>& amounts);

	void sendResponse(int sockfd, Response response);
	void recvRequest(int sockfd, Request& request);
	bool processRequest(Request request, Response& response);
	bool serveRequests(int sockfd);

private:
	ClientInfo client_info_;
	MYSQL *conn_;
	MYSQL_RES *res_;
	MYSQL_ROW row_;

	std::stack<Request> undostack_;
    std::stack<Request> redostack_;
	ThreadSafeQueue<Transaction>* loggingQueue_;
};

#endif