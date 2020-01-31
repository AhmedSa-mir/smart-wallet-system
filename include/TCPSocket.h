
#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class TCPSocket {
public:
	TCPSocket(int port);
	~TCPSocket();

	int createSocket();
	int getSockfd();
	
protected:	
	const int port_;
	int sockfd_;
	struct sockaddr_in addr_;
};

#endif