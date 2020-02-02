
#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <sstream>
#include <errno.h>
#include <signal.h>
#include <stack>

#include "communication.h"
#include "client_TCPSocket.h"

class Client {
public:
	Client(int port, std::string IP);
	~Client();

	int createSocket();
	int connectToServer();
    void closeSockfd();

    void sendRequest(Request request);
    void recvResponse(Response& response);

    bool isValidId(std::string id);
    bool isValidAge(int age);

    void setRequestType(REQUEST_TYPE type);
    REQUEST_TYPE getRequestType();

    void pushUndoStack(const Request& request);
    void pushRedoStack(const Request& request);
    Request popUndoStack();
    Request popRedoStack();
    bool isUndoStackEmpty();
    bool isRedoStackEmpty();

private:
	ClientTCPSocket* socket_;
    REQUEST_TYPE request_type_;
    std::stack<Request> undostack_;
    std::stack<Request> redostack_;
};

#endif
