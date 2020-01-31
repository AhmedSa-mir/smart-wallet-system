
#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <sstream>
#include <errno.h>
#include <signal.h>

#include "communication.h"
#include "client_TCPSocket.h"

class Client {
public:
	Client(int port, std::string IP);
	~Client();

	int createSocket();
	int connectToServer();

	void run();
	void serveClientRequests();
	void sendRequest();
	void recvResponse();
	void clearRequest();

	void login();
	void deposit();
	void withdraw();
	unsigned long checkBalance();

	void bye();
	
private:
	ClientTCPSocket* socket_;
	Request request_;
	Response response_;
};

#endif