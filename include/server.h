#include <pthread.h>

#include "message.h"
#include "server_TCPSocket.h"

class Server {
public:
	Server(int port);
	~Server();
	void createSocket();
	void bindSocketToAddress();
	void listenOnSocket();
	int acceptConnection();
	void serve();
private:
	ServerTCPSocket* socket_;
};