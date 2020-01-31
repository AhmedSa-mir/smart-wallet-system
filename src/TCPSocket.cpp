#include "TCPSocket.h"

TCPSocket::TCPSocket(int port) : port_(port)
{
	memset(&addr_, 0, sizeof(addr_));
}

TCPSocket::~TCPSocket()
{
	close(sockfd_);
}

int TCPSocket::createSocket()
{
	sockfd_ = socket(AF_INET, SOCK_STREAM, 0);	// Create TCP(SOCK_STREAM) socket
	if (sockfd_ < 0)
	{
		perror("Error");
		return -1;
	}
	return 0;
}

int TCPSocket::getSockfd()
{
	return sockfd_;
}