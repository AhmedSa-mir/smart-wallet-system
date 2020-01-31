
#ifndef CLIENT_TCPSOCKET_H_
#define CLIENT_TCPSOCKET_H_

#include "TCPSocket.h"

class ClientTCPSocket : public TCPSocket {
public:
	ClientTCPSocket(int port, std::string IP);
	~ClientTCPSocket();

	int connectToServer();
	
private:	
	const std::string server_ip_;
};

#endif