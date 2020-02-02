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

void Client::closeSockfd()
{
    close(socket_->getSockfd());
}

// Sends request message to server
void Client::sendRequest(Request request)
{
    int sockfd = socket_->getSockfd();

    // Setup request (type + message data)
    std::string msg_data(request.data);
    std::string request_content = std::to_string(request.type) + " " + msg_data;

    // Send message size first
    sprintf(request.data,"%ld", request_content.length());

    int bytes_cnt = write(sockfd, request.data, sizeof(request.size));
    if (bytes_cnt < 0)
    {
        std::cerr << "ERROR writing to socket\n";
    }

    // Send message
    strcpy(request.data, request_content.c_str());
    request.size = request_content.length();

    bytes_cnt = write(sockfd, request.data, request.size);
    if (bytes_cnt < 0)
    {
        std::cerr << "ERROR writing to socket\n";
    }

    std::cout << "Request sent: " << request_content << std::endl;
}

// Receives response message from the server
void Client::recvResponse(Response& response)
{
    int sockfd = socket_->getSockfd();

    memset(response.data, 0, MSG_MAX_SIZE);

    // Receive message size first
    int bytes_cnt = read(sockfd, response.data, sizeof(response.size));
    if (bytes_cnt < 0)
    {
        std::cerr << "ERROR reading from socket\n";
    }

    int msg_size = std::stoi(response.data);
    std::cout << "Client Received " << msg_size << std::endl;

    // Receive message
    bytes_cnt = read(sockfd, response.data, msg_size);
    if (bytes_cnt < 0)
    {
        std::cerr << "ERROR reading from socket\n";
    }

    std::cout << "Client Received: " << response.data << std::endl;

    std::string data(response.data);

    // Split data by spaces
    std::istringstream iss(data);
    std::string status, raw_data;
    std::getline(iss, status, ' ');
    std::getline(iss, raw_data, ' ');

    std::cout << "Status received: " << status << std::endl;
    std::cout << "raw_data received: " << raw_data << std::endl;

    response.status = RESPONSE_STATUS(std::stoi(status));

    strcpy(response.data, raw_data.c_str());
}


bool Client::isValidId(std::string id)
{
   return (id.find_first_not_of("0123456789") == std::string::npos)
            && (id.length() == 14);
}

bool Client::isValidAge(int age)
{
    // TODO: Check age correctly
    return (age >= 18 && age < 200);
}

void Client::setRequestType(REQUEST_TYPE type)
{
    request_type_ = type;
}

REQUEST_TYPE Client::getRequestType()
{
    return request_type_;
}

void Client::pushUndoStack(const Request& request)
{
    undostack_.push(request);
}

void Client::pushRedoStack(const Request& request)
{
    redostack_.push(request);
}

Request Client::popUndoStack()
{
    Request request = undostack_.top();
    undostack_.pop();
    return request;
}

Request Client::popRedoStack()
{
    Request request = redostack_.top();
    redostack_.pop();
    return request;
}

bool Client::isUndoStackEmpty()
{
    return undostack_.empty();
}

bool Client::isRedoStackEmpty()
{
    return redostack_.empty();
}
