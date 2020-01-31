#include "client.h"

Client::Client(int port, std::string IP)
{
	socket_ = new ClientTCPSocket(port, IP);
}

Client::~Client()
{
	delete socket_;
}

int Client::createSocket()
{
	return socket_->createSocket();
}

int Client::connectToServer()
{
	return socket_->connectToServer();
}

void Client::run()
{
	int sockfd = connectToServer();
	if(sockfd < 0)
	{
		std::cout << "Error on connect\n";
	}
	else
	{
		login();

		serveClientRequests();

		close(sockfd);
	}
}

void Client::clearRequest()
{
	memset(request_.data, 0, MSG_MAX_SIZE);
	request_.size = 0;
	request_.type = NONE;
}

void Client::bye()
{
	request_.type = BYE;
	memset(request_.data, 0, MSG_MAX_SIZE);
	request_.size = 0;

	sendRequest();
}

void Client::serveClientRequests()
{
	unsigned int option = 4;
	
	while(true)
	{
		std::cout << "Type 1 to check balance\n"
				  << "Type 2 to deposit money\n"
				  << "Type 3 to withdraw money\n"
				  << "Type 4 to exit\n";
		std::cin >> option;

		switch (option)
		{
		case 1:
			checkBalance();
			break;
		case 2:
			deposit();
			break;
		case 3:
			withdraw();
			break;
		case 4:
			bye();
			return;
		default:
			std::cout << "Please enter a valid number!\n";
		}
	}
}

void Client::login()
{
	unsigned int option = 3;
	std::string name = "";
	std::string national_id = "";
	long long age = -1;

	do
	{
		std::cout << "Welcome!\n";
		std::cout << "Type 1 to login\n"
				  << "Type 2 to register a new user\n";
		std::cin >> option;

		// Login with national id
		if(option == 1)
		{
			while(national_id.length() != 14)
			{ 
				std::cout << "Enter your national id: ";
				std::cin >> national_id;

				if(national_id.length() != 14)
				{
					std::cout << "Invalid id. National IDs are 14 characters!\n";
				}
			}

			clearRequest();

			request_.type = LOGIN;
			strcpy(request_.data, national_id.c_str());
			request_.size = strlen(request_.data);

			sendRequest();
			recvResponse();

			if(response_.status == SUCCESS)
			{
				std::string name(response_.data);

				std::cout << "Welcome back " << name << " :)\n";
			}
			else if(response_.status == INVALID_ID)
			{
				std::cout << "Server response: Invalid ID!\n";
			}
			else if(response_.status == FAIL)
			{
				std::cout << "Server response: Fail!\n";
			}
		}
		// Register new user
		else if (option == 2)
		{
			std::cout << "Enter your name: ";
			std::cin >> name;

			while(national_id.length() != 14)
			{ 
				std::cout << "Enter your national id: ";
				std::cin >> national_id;

				if(national_id.length() != 14)
				{
					std::cout << "Invalid id. National IDs are 14 characters!\n";
				}
			}

			while(age <= 0 || age > 200)
			{
				std::cout << "Enter your age: ";
				std::cin >> age;

				if(age <= 0 || age > 200)
				{
					std::cout << "Invalid age!\n";
				}
			}

			clearRequest();

			request_.type = REGISTER;
			std::string msg_content = name + " " + national_id + " " + std::to_string(age);
			strcpy(request_.data, msg_content.c_str());
			request_.size = strlen(request_.data);

			sendRequest();
			recvResponse();

			if(response_.status == SUCCESS)
			{
				std::string name(response_.data);

				std::cout << "Welcome " << name << " :)\n";
			}
			else if(response_.status == INVALID_ID)
			{
				std::cout << "Server response: Invalid ID!\n";
			}
			else if(response_.status = INVALID_AGE)
			{
				std::cout << "Server response: Invalid age!\n";
			}
			else if(response_.status == FAIL)
			{
				std::cout << "Server response: Fail!\n";
			}
		}
		else
		{
			std::cout << "Please enter 1 or 2\n";
		}
	}while(option != 1 && option != 2);
}

void Client::deposit(){
	
	long long amount = -1;

	do
	{
		std::cout << "Enter the amount you want to deposit: ";
		std::cin >> amount;
		if (amount <= 0)
		{
			std::cout << "Please enter a valid amount\n";
		}
	}while(amount <= 0);

	clearRequest();

	request_.type = DEPOSIT;
	strcpy(request_.data, std::to_string(amount).c_str());
	request_.size = strlen(request_.data);

	sendRequest();
	recvResponse();

	if(response_.status == SUCCESS)
	{
		std::cout << "Transaction done successfully\n";
	}
	else if(response_.status == INVALID_AMOUNT)
	{
		std::cout << "Server response: Invalid amount!\n";
	}
	else if(response_.status == FAIL)
	{
		std::cout << "Server response: Fail!\n";
	}
}

void Client::withdraw()
{
	long long amount = -1;

	do
	{
		std::cout << "Enter the amount you want to withdraw: ";
		std::cin >> amount;
		if (amount <= 0)
		{
			std::cout << "Please enter a valid amount\n";
		}
	}while(amount <= 0);

	clearRequest();

	request_.type = WITHDRAW;
	strcpy(request_.data, std::to_string(amount).c_str());
	request_.size = strlen(request_.data);

	sendRequest();
	recvResponse();
	
	if(response_.status == SUCCESS)
	{
		std::cout << "Transaction done successfully\n";
	}
	else if(response_.status == INVALID_AMOUNT)
	{
		std::cout << "Server response: Invalid amount!\n";
	}
	else if(response_.status == NOT_ENOUGH_MONEY)
	{
		std::cout << "Server response: Not enough money!\n";
	}
	else if(response_.status == FAIL)
	{
		std::cout << "Server response: Fail!\n";
	}
}

unsigned long Client::checkBalance()
{
	clearRequest();

	request_.type = CHECK_BALANCE;
	memset(request_.data, 0, MSG_MAX_SIZE);
	request_.size = 0;

	sendRequest();
	recvResponse();

	if(response_.status == SUCCESS)
	{
		std::string balance(response_.data);
		std::cout << "Your current balance is " << balance << std::endl;
	}
	else if(response_.status == FAIL)
	{
		std::cout << "Server response: Fail!\n";
	}
}

// Sends request message to server
void Client::sendRequest()
{

	int sockfd = socket_->getSockfd();

	// Setup request (type + message data)
	std::string msg_data(request_.data);
	std::string request_content = std::to_string(request_.type) + " " + msg_data;

	// Send message size first
	sprintf(request_.data,"%ld", request_content.length());

	int bytes_cnt = write(sockfd, request_.data, sizeof(request_.size));
	if (bytes_cnt < 0)
	{
		std::cerr << "ERROR writing to socket\n";
	}

	// Send message
	strcpy(request_.data, request_content.c_str());
	request_.size = request_content.length();

	bytes_cnt = write(sockfd, request_.data, request_.size);
	if (bytes_cnt < 0)
	{
		std::cerr << "ERROR writing to socket\n";
	}

	std::cout << "Request sent: " << request_content << std::endl;
}

// Receives response message from the server
void Client::recvResponse()
{
	int sockfd = socket_->getSockfd();

	memset(response_.data, 0, MSG_MAX_SIZE);

	// Receive message size first
	int bytes_cnt = read(sockfd, response_.data, sizeof(response_.size));
	if (bytes_cnt < 0)
	{
		std::cerr << "ERROR reading from socket\n";
	}

	int msg_size = std::stoi(response_.data);
	std::cout << "Client Received " << msg_size << std::endl;

	// Receive message
	bytes_cnt = read(sockfd, response_.data, msg_size);
	if (bytes_cnt < 0)
	{
		std::cerr << "ERROR reading from socket\n";
	}

	std::cout << "Client Received: " << response_.data << std::endl;

	std::string data(response_.data);

	// Split data by spaces
	std::istringstream iss(data);
	std::string status, raw_data;
	std::getline(iss, status, ' ');
	std::getline(iss, raw_data, ' ');

	std::cout << "Status received: " << status << std::endl;
	std::cout << "raw_data received: " << raw_data << std::endl;

	response_.status = RESPONSE_STATUS(std::stoi(status));

	strcpy(response_.data, raw_data.c_str());
}