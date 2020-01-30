#include <iostream>
#include "server_TCPSocket.h"

int main(int argc, char *argv[])
{
	int new_sockfd;
	char buffer[256];
	int bytes_cnt;

	// Port argument required
	if (argc < 2) {
		std::cerr << "Usage: ./server <port>\n";
		return -1;
	}

	// Get port number (passed as argument)
	int port = std::stoi(argv[1]);

	ServerTCPSocket socket(port);

	socket.createSocket();

	socket.bindSocketToAddress();

	socket.listenOnSocket();

	new_sockfd = socket.acceptConnection();

	memset(buffer, 0, 256);

	bytes_cnt = read(new_sockfd, buffer, 255);
	if (bytes_cnt < 0) {
		std::cerr << "ERROR reading from socket\n";
	}
		
	std::cout << "Here is the message: " << buffer << std::endl;

	bytes_cnt = write(new_sockfd, "I got your message", 18);
	if (bytes_cnt < 0) {
		std::cerr << "ERROR writing to socket\n";
	}

	close(new_sockfd);

	return 0; 
}