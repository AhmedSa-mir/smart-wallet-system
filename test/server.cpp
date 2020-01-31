#include <iostream>

#include "server.h"

int main(int argc, char *argv[])
{
	// Port argument required
	if (argc < 2) {
		std::cerr << "Usage: ./server <port>\n";
		return -1;
	}

	// Get port number (passed as argument)
	int port = std::stoi(argv[1]);

	Server server(port);

	server.createSocket();

	server.bindSocketToAddress();

	server.listenOnSocket();

	// Serve clients (infinite loop)
	server.serve();

	return 0; 
}