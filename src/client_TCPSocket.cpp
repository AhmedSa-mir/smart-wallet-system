#include "client_TCPSocket.h"

ClientTCPSocket::ClientTCPSocket(int port, std::string IP)
	: TCPSocket(port), server_ip_(IP){}

ClientTCPSocket::~ClientTCPSocket(){}

int ClientTCPSocket::connectToServer()
{
	addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port_);     // hotns = puts port in network byte order

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, server_ip_.c_str(), &addr_.sin_addr) <= 0)
    { 
        perror("Error");
        return -1; 
    }

    if (connect(sockfd_,(struct sockaddr *) &addr_,sizeof(addr_)) < 0)
    {
        perror("Error");
        return -1;
    }
    
    return sockfd_;
}
