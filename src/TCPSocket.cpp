#include "TCPSocket.h"

TCPSocket::TCPSocket(int port) : port_(port) {
	memset(&addr_, 0, sizeof(addr_));
}

TCPSocket::~TCPSocket(){
	close(sockfd_);
}

void TCPSocket::createSocket(){
	sockfd_ = socket(AF_INET, SOCK_STREAM, 0);	// Create TCP(SOCK_STREAM) socket
	if (sockfd_ < 0) {
		std::cerr << "ERROR opening socket\n";
	}
}