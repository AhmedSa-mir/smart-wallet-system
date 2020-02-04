
#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <sstream>
#include <fstream>
#include <errno.h>

#include "defs.h"
#include "server_defs.h"
#include "client_handler.h"
#include "server_TCPSocket.h"
#include "thread-safe_queue.h"

class Server {
public:
	Server(int port);
	~Server();

	int createSocket();
	int bindSocketToAddress();
	int listenOnSocket();
	int acceptConnection();

	void run(DBParams db_params);
	void handleClient(int sockfd, DBParams db_params);

	void logTransactions();

private:
	ServerTCPSocket* socket_;
	ThreadSafeQueue<Transaction> logging_queue;
};

#endif
