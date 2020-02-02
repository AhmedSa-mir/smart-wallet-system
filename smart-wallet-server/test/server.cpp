#include <iostream>

#include "server.h"


int main(int argc, char *argv[])
{
	// Port argument required
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <port>\n";
		return -1;
	}

	// Get port number (passed as argument)
	int port = std::stoi(argv[1]);

	Server server(port);

	int ret = server.createSocket();
	if(ret == -1)
	{
		std::cout << "Error on create socket\n";
		return -1;
	}

	ret = server.bindSocketToAddress();
	if(ret == -1)
	{
		std::cout << "Error on bind\n";
		return -1;
	}

	ret = server.listenOnSocket();
	if(ret == -1)
	{
		std::cout << "Error on listen\n";
		return -1;
	}

	// Serve clients (infinite loop)
	server.run();

	return 0; 
}