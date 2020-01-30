#include <iostream>
#include "client_TCPSocket.h"

int main(int argc, char *argv[])
{
    int sockfd;
    char buffer[256];
    int bytes_cnt;

    // Server IP & Port arguments required
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <port>\n";
        return -1;
    }

    // Get port number (passed as argument)
    std::string server_ip(argv[1]);
    int port = std::stoi(argv[2]);

    ClientTCPSocket socket(port, server_ip);

    socket.createSocket();
    
    sockfd = socket.connectToServer();

    std::cout << "Please enter the message: ";

    memset(buffer,0,256);
    std::cin >> buffer;

    bytes_cnt = write(sockfd, buffer, strlen(buffer));
    if (bytes_cnt < 0) {
        std::cerr << "ERROR writing to socket\n";
    }
    
    memset(buffer,0,256);
    
    bytes_cnt = read(sockfd, buffer, 255);
    if (bytes_cnt < 0) {
        std::cerr << "ERROR reading from socket\n";
    }

    std::cout << buffer << std::endl;

    close(sockfd);

    return 0;
}