
#ifndef SERVER_TCPSOCKET_H_
#define SERVER_TCPSOCKET_H_

#include "TCPSocket.h"

// Number of connections that can be waiting while the process
// is handling a particular connection (5 is the max backlog queue size) 
#define BACKLOG_QUEUE_SIZE 5

class ServerTCPSocket : public TCPSocket {
public:
	ServerTCPSocket(int port);
	~ServerTCPSocket();
	
	int bindSocketToAddress();
	int listenOnSocket();
	int acceptConnection();
};

#endif