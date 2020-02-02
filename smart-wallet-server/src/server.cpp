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

void Server::logTransactions()
{
	std::ofstream logFile;
	logFile.open("transactions.log", std::fstream::out | std::fstream::app);

	Transaction transaction;
	while(true)
	{

		loggingQueue.pop(transaction);	// block for a transaction
		std::cout << "LOOOG\n";
		std::string request_type;
		if(transaction.type == DEPOSIT)
		{
			request_type = "DEPOSIT";
		}
		else if(transaction.type == WITHDRAW)
		{
			request_type = "WITHDRAW";
		}

		logFile << transaction.id << " "
				<< request_type << " "
				<< transaction.amount << std::endl;
	}

	logFile.close();
}

void Server::handleClient(int sockfd)
{
	ClientHandler client_handler(&loggingQueue);

	// Keep serving client requests until receiving bye
	while(client_handler.serveRequests(sockfd)){};

	close(sockfd);

	std::cout << "Client handler thread exiting\n";
}

void Server::run()
{
	// Spawn a thread to log transactions
	std::thread logging_thread(&Server::logTransactions, this);

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