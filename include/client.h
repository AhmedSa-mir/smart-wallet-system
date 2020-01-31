#include <iostream>

#include "message.h"
#include "client_TCPSocket.h"

class Client {
public:
	Client(int port, std::string IP);
	~Client();
	void createSocket();
	int connectToServer();
	void run();
private:
	ClientTCPSocket* socket_;
	Message request_;
	Message response_;
};