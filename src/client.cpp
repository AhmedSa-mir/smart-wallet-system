#include "client.h"

Client::Client(int port, std::string IP){
	socket_ = new ClientTCPSocket(port, IP);
}

Client::~Client(){
	delete socket_;
}

void Client::createSocket(){
	socket_->createSocket();
}

int Client::connectToServer() {
	return socket_->connectToServer();
}

void Client::run() {

	int sockfd = connectToServer();
	
	memset(request_.data, 0, sizeof(request_.size));
	request_.size = 0;

	char* send_msg = "CM233asassa12";

	// Send message size first
	sprintf(request_.data,"%ld", strlen(send_msg));

	int bytes_cnt = write(sockfd, request_.data, sizeof(request_.size));
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

	memset(response_.data, 0, MSG_MAX_SIZE);

	// Receive message size first
	bytes_cnt = read(sockfd, response_.data, sizeof(response_.size));
	if (bytes_cnt < 0) {
		std::cerr << "ERROR reading from socket\n";
	}

	int msg_size = std::stoi(response_.data);
	std::cout << "Client Received " << msg_size << std::endl;

	// Receive message
	bytes_cnt = read(sockfd, response_.data, msg_size);
	if (bytes_cnt < 0) {
		std::cerr << "ERROR reading from socket\n";
	}

	std::cout << "Client Received: " << response_.data << std::endl;

	close(sockfd);
}