#include <iostream>

#include "client.h"

int main(int argc, char *argv[])
{
    // Server IP & Port arguments required
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <IP> <port>\n";
        return -1;
    }

    // Get port number (passed as argument)
    std::string server_ip(argv[1]);
    int port = std::stoi(argv[2]);

    Client client(port, server_ip);

    int ret = client.createSocket();
    if(ret == -1)
    {
        std::cout << "Error on create socket\n";
        return -1;
    }

    // Request services from the server
    client.run();

    return 0;
}