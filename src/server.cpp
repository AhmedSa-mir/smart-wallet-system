#include "server.h"

Server::Server(int port)
{
	socket_ = new ServerTCPSocket(port);
}

Server::~Server()
{
	delete socket_;
}

int Server::createSocket()
{
	return socket_->createSocket();
}

int Server::bindSocketToAddress()
{
	return socket_->bindSocketToAddress();
}

int Server::listenOnSocket()
{
	return socket_->listenOnSocket();
}

int Server::acceptConnection()
{
	return socket_->acceptConnection();
}

void Server::handleClient(int sockfd)
{
	ClientHandler client_handler;

	// Keep serving client requests until receiving bye
	while(client_handler.serveRequests(sockfd)){};

	close(sockfd);

	std::cout << "Client handler thread exiting\n";

	pthread_exit(NULL);
}

void Server::serve()
{
	while(true)
	{
		int new_sockfd = acceptConnection();
		if(new_sockfd == -1)
		{
			std::cout << "Error on accept\n";
		}
		else
		{
			// Spawn a thread to serve the client
			std::thread th(&Server::handleClient, this, new_sockfd);
			th.detach();
		}
	}
}