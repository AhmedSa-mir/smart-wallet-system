
#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <sstream>
#include <errno.h>
#include <signal.h>
#include <stack>
#include <vector>

#include "defs.h"
#include "client_TCPSocket.h"

class Client {
public: 
    Client(int port, std::string IP);
    ~Client();
    
    int createSocket();
    int connectToServer();
    void closeSockfd();

    void setRequestType(REQUEST_TYPE type);
    void sendRequest(Request request);
    void recvResponse(Response& response);

    bool isValidId(std::string id);
    bool isValidAge(int age);

    RESPONSE_STATUS login(std::string national_id, std::string& name);
    RESPONSE_STATUS register_new_client(std::string name,
                                        std::string national_id,
                                        std::string age,
                                        std::string gender);
    RESPONSE_STATUS checkBalance(unsigned long long& balance);
    RESPONSE_STATUS sendTransaction(unsigned long long amount);
    RESPONSE_STATUS undo(unsigned long long& amount, bool& empty_stack, REQUEST_TYPE& type);
    RESPONSE_STATUS redo(unsigned long long& amount, bool& empty_stack, REQUEST_TYPE& type);
    RESPONSE_STATUS getStats(std::string& month_rates);

    void bye();

private:
    ClientTCPSocket* socket_;
    REQUEST_TYPE request_type_;
    std::stack<Request> undostack_;
    std::stack<Request> redostack_;
};

#endif
