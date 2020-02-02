
#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <sstream>
#include <errno.h>

#include "communication.h"
#include "client_info.h"
#include "client_handler.h"
#include "server_TCPSocket.h"

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

private:
	ServerTCPSocket* socket_;
};

#endif