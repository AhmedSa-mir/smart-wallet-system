#include "server_TCPSocket.h"

ServerTCPSocket::ServerTCPSocket(int port) : TCPSocket(port) {
}

ServerTCPSocket::~ServerTCPSocket(){
}

void ServerTCPSocket::bindSocketToAddress(){
	addr_.sin_family = AF_INET;
	addr_.sin_addr.s_addr = INADDR_ANY;	// IP of the host machine
	addr_.sin_port = htons(port_);		// hotns = puts port in network byte order

	// Bind socket to the serv_address
	if (bind(sockfd_, (struct sockaddr *) &addr_, sizeof(addr_)) < 0) {
		std::cerr << "ERROR on binding\n";
	}
}

void ServerTCPSocket::listenOnSocket(){
	// 5 is the max backlog queue size i.e., the number of connections that can be
	// waiting while the process is handling a particular connection
	listen(sockfd_, 5);
}	

int ServerTCPSocket::acceptConnection(){
	socklen_t client_addr_len = sizeof(addr_);
	int new_sockfd = accept(sockfd_, (struct sockaddr *) &addr_, &client_addr_len);
	if (new_sockfd < 0) {
		std::cerr << "ERROR on accept\n";
		return -1;
	}
	return new_sockfd;
}
