#include "server.h"

Server::Server(int port){
	socket_ = new ServerTCPSocket(port);
}

Server::~Server(){
	delete socket_;
}

void Server::createSocket(){
	socket_->createSocket();
}

void Server::bindSocketToAddress(){
	socket_->bindSocketToAddress();
}

void Server::listenOnSocket(){
	socket_->listenOnSocket();
}

int Server::acceptConnection(){
	return socket_->acceptConnection();
}

void* handleClient(void* args){

	int sockfd = *((int*)args);

	Message request_;
	Message response_;

	memset(response_.data, 0, MSG_MAX_SIZE);

	// Receive message size first
	int bytes_cnt = read(sockfd, response_.data, sizeof(response_.size));
	if (bytes_cnt < 0) {
		std::cerr << "ERROR reading from socket\n";
	}

	int msg_size = std::stoi(response_.data);
	std::cout << "Client handler Received: " << msg_size << std::endl;

	// Receive message
	bytes_cnt = read(sockfd, response_.data, msg_size);
	if (bytes_cnt < 0) {
		std::cerr << "ERROR reading from socket\n";
	}

	std::cout << "Client handler Received: " << response_.data << std::endl;

	memset(request_.data, 0, MSG_MAX_SIZE);

	char* send_msg = "SM2222";

	// Send message size first
	sprintf(request_.data,"%ld", strlen(send_msg));

	bytes_cnt = write(sockfd, request_.data, sizeof(request_.size));
	if (bytes_cnt < 0) {
		std::cerr << "ERROR writing to socket\n";
	}

	// Send message
	strcpy(request_.data, send_msg);
	request_.size = strlen(send_msg);

	bytes_cnt = write(sockfd, request_.data, request_.size);
	if (bytes_cnt < 0) {
		std::cerr << "ERROR writing to socket\n";
	}

	close(sockfd);

	pthread_exit(NULL);
}

void Server::serve(){

	while(true){

		int new_sockfd = acceptConnection();

		// Spawn a thread to serve the client
		pthread_t thread;

		int err = pthread_create(&thread, NULL,
								 handleClient, &new_sockfd);
		if (err) {
			std::cerr << "Thread creation failed\n";
		}
		else {
			std::cout << "Client handler thread created\n";
		}
	}
}