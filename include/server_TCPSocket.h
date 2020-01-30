#include "TCPSocket.h"

class ServerTCPSocket : public TCPSocket {
public:
	ServerTCPSocket(int port);
	~ServerTCPSocket();
	void bindSocketToAddress();
	void listenOnSocket();
	int acceptConnection();
};