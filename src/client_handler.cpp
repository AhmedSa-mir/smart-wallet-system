#include "client_handler.h"

ClientHandler::ClientHandler(){}
ClientHandler::~ClientHandler(){}

ClientInfo ClientHandler::getClientInfo(unsigned long national_id)
{
	// TODO: Implement this method

	ClientInfo client_info;

	client_info.name = "name";
	client_info.national_id = national_id;
	client_info.age = 15;
	client_info.balance = getBalance();

	return client_info;
}

bool ClientHandler::isValidId(std::string id)
{
	// TODO: Check id correctly
	return (id.length() == 14);
}

bool ClientHandler::isValidAge(int age)
{
	// TODO: Check age correctly
	return (age > 0 && age < 200);
}

bool ClientHandler::createNewAccount(const ClientInfo& client_info)
{
	// TODO: Implement this method
	return true;
}

bool ClientHandler::deposit(unsigned long long balance)
{
	// TODO: Implement this method
	return true;
}

bool ClientHandler::withdraw(unsigned long long balance)
{
	// TODO: Implement this method
	return true;
}

unsigned long long ClientHandler::getBalance()
{
	// TODO: Implement this method
	return 0;
}

void ClientHandler::sendResponse(Response response, int sockfd)
{
	// Setup response (status + message data)
	std::string msg_data(response.data);
	std::string response_content = std::to_string(response.status) + " " + msg_data;
	
	// Send message size first
	sprintf(response.data,"%ld", response_content.length());

	int bytes_cnt = write(sockfd, response.data, sizeof(response.size));
	if (bytes_cnt < 0)
	{
		perror("Write Socket Error");
	}

	// Send message
	strcpy(response.data, response_content.c_str());
	response.size = response_content.length();

	bytes_cnt = write(sockfd, response.data, response.size);
	if (bytes_cnt < 0)
	{
		perror("Write Socket Error");
	}

	std::cout << "Response sent: " << response_content << std::endl;
}

std::string ClientHandler::recvRequest(int sockfd)
{
	Request request;

	memset(request.data, 0, MSG_MAX_SIZE);
	request.size = 0;

	// Receive message size first
	int bytes_cnt = read(sockfd, request.data, sizeof(request.size));
	if (bytes_cnt < 0)
	{
		perror("Read Socket Error");
	}

	int msg_size = std::atoi(request.data);
	std::cout << "Client handler Received: " << msg_size << std::endl;

	// Receive message
	bytes_cnt = read(sockfd, request.data, msg_size);
	if (bytes_cnt < 0)
	{
		perror("Read Socket Error");
	}

	std::cout << "Client handler Received: " << request.data << std::endl;

	return std::string(request.data);
}

bool ClientHandler::processRequest(std::string data, Response& response)
{
	// Split data by spaces
	std::istringstream iss(data);
	std::string msg_type, raw_data;
	std::getline(iss, msg_type, ' ');
	std::getline(iss, raw_data, ' ');

	std::cout << "Type received: " << msg_type << std::endl;

	REQUEST_TYPE type = REQUEST_TYPE(std::stoi(msg_type));
	if(type == LOGIN)
	{
		std::string national_id = raw_data;

		if(!isValidId(national_id))
		{
			std::cout << "Invalid id. National IDs are 14 characters!\n";
			response.status = INVALID_ID;
		}
		else
		{
			ClientInfo client_info = getClientInfo(std::stoul(national_id));

			// Send success + client name
			response.status = SUCCESS;
			strcpy(response.data, client_info.name.c_str());
			response.size = strlen(response.data);
		}
	}
	else if(type == REGISTER)
	{
		std::string name, national_id, age;
		name = raw_data;
		std::getline(iss, national_id, ' ');
		std::getline(iss, age, ' ');

		std::cout << "name: " << name << std::endl;
		std::cout << "national_id: " << national_id << std::endl;
		std::cout << "age: " << age << std::endl;

		if(!isValidId(national_id))
		{
			std::cout << "Invalid id. National IDs are 14 characters!\n";
			response.status = INVALID_ID;
		}
		else if(!isValidAge(std::stoi(age)))
		{
			std::cout << "Invalid age!\n";
			response.status = INVALID_AGE;
		}
		else
		{
			// Create new user
			ClientInfo client_info;
			client_info.name = "name";
			client_info.national_id = std::stoul(national_id);
			client_info.age = 15;
			client_info.balance = 0;

			bool err = !createNewAccount(client_info);
			if(err)
			{
				std::cout << "Error creating new account\n";
				response.status = FAIL;
			}

			// Send success + client name
			response.status = SUCCESS;
			strcpy(response.data, client_info.name.c_str());
			response.size = strlen(response.data);
		}
	}
	else if(type == DEPOSIT)
	{
		unsigned long long amount = std::stoll(raw_data);

		if (amount <= 0)
		{
			std::cout << "Invalid amount!\n";
			response.status = INVALID_AMOUNT;
		}
		else
		{
			deposit(amount);
			response.status = SUCCESS;
		}
	}
	else if(type == WITHDRAW)
	{
		unsigned long long amount = std::stoll(raw_data);

		if (amount <= 0)
		{
			std::cout << "Invalid amount!\n";
			response.status = INVALID_AMOUNT;
		}
		else
		{	
			// Check for balance first
			unsigned long long balance = getBalance();

			if(amount > balance)
			{
				std::cout << "Not enough money in your account!\n";
				response.status = NOT_ENOUGH_MONEY;
			}
			else
			{
				withdraw(amount);
				response.status = SUCCESS;
			}
		}
	}
	else if(type == CHECK_BALANCE)
	{
		unsigned long long balance = getBalance();

		// Send success + balance
		response.status = SUCCESS;
		strcpy(response.data,  std::to_string(balance).c_str());
		response.size = strlen(response.data);
	}
	else if(type == BYE)
	{		
		std::cout << "Client exited!\n";
		return false;
	}

	return true;
}

bool ClientHandler::serveRequests(int sockfd)
{
	Response response;
	memset(response.data, 0, MSG_MAX_SIZE);
	response.size = 0;

	std::string data = recvRequest(sockfd);

	bool exit = !processRequest(data, response);
	if(!exit)
	{
		sendResponse(response, sockfd);
		return true;
	}

	return false;
}