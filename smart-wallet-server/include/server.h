
#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <sstream>
#include <fstream>
#include <errno.h>

#include "defs.h"
#include "client_info.h"
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

	void run();
	void handleClient(int sockfd);

	void logTransactions();

private:
	ServerTCPSocket* socket_;
	ThreadSafeQueue<Transaction> loggingQueue;
};

#endif