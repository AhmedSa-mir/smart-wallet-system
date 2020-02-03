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

void Client::setRequestType(REQUEST_TYPE type)
{
    request_type_ = type;
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

    // Separate response status from data
    std::string data(response.data);
    int delemiter = data.find(" ", 0);
    std::string status = data.substr(0, delemiter);
    std::string raw_data = data.substr(delemiter + 1);
    strcpy(response.data, raw_data.c_str());

    std::cout << "Status received: " << status << std::endl;
    std::cout << "raw_data received: " << raw_data << std::endl;

    response.status = RESPONSE_STATUS(std::stoi(status));
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

RESPONSE_STATUS Client::login(std::string national_id, std::string& response_msg)
{
    Request request;
    Response response;

    request.type = LOGIN;
    strcpy(request.data, national_id.c_str());
    request.size = strlen(request.data);

    sendRequest(request);
    recvResponse(response);

    response_msg.assign(response.data);

    return response.status;
}

RESPONSE_STATUS Client::register_new_client(std::string name,
                                            std::string national_id,
                                            std::string age,
                                            std::string gender)
{
    Request request;
    Response response;

    request.type = REGISTER;
    std::string msg_content = name + " " + national_id + " " + age + " " + gender;
    strcpy(request.data, msg_content.c_str());
    request.size = strlen(request.data);

    sendRequest(request);
    recvResponse(response);

    return response.status;
}

RESPONSE_STATUS Client::checkBalance(unsigned long long& balance)
{
    Request request;
    Response response;

    request.type = CHECK_BALANCE;
    memset(request.data, 0, MSG_MAX_SIZE);
    request.size = 0;

    sendRequest(request);
    recvResponse(response);

    if(response.status == SUCCESS)
    {
        balance = std::stoull(response.data);
    }

    return response.status;
}

RESPONSE_STATUS Client::sendTransaction(unsigned long long amount)
{
    Request request;
    Response response;

    request.type = request_type_;
    strcpy(request.data, std::to_string(amount).c_str());
    request.size = strlen(request.data);

    sendRequest(request);
    recvResponse(response);

    if(response.status == SUCCESS)
    {
        undostack_.push(request);
    }

    return response.status;
}

RESPONSE_STATUS Client::undo(unsigned long long& amount, bool& empty_stack, REQUEST_TYPE& type)
{
    Request undo_request;
    Response undo_response;

    undo_request.type = UNDO;
    memset(undo_request.data, 0, MSG_MAX_SIZE);
    undo_request.size = 0;

    sendRequest(undo_request);
    recvResponse(undo_response);

    if(undo_response.status == SUCCESS)
    {
        Request undo_request = undostack_.top();
        amount = std::stoull(undo_request.data);
        type = undo_request.type;

        redostack_.push(undo_request);
        undostack_.pop();
        if(undostack_.empty())
        {
            empty_stack = true;
        }
    }

    return undo_response.status;
}

RESPONSE_STATUS Client::redo(unsigned long long& amount, bool& empty_stack, REQUEST_TYPE& type)
{
    Request redo_request;
    Response redo_response;

    redo_request.type = REDO;
    memset(redo_request.data, 0, MSG_MAX_SIZE);
    redo_request.size = 0;

    sendRequest(redo_request);
    recvResponse(redo_response);

    if(redo_response.status == SUCCESS)
    {
        Request redo_request = redostack_.top();
        amount = std::stoull(redo_request.data);
        type = redo_request.type;

        redostack_.pop();
        if(redostack_.empty())
        {
            empty_stack = true;
        }
    }

    return redo_response.status;
}

void Client::bye()
{
    Request request;

    request.type = BYE;
    memset(request.data, 0, MSG_MAX_SIZE);
    request.size = 0;

    sendRequest(request);
}
