#include <iostream>

#include "server.h"

int main(int argc, char *argv[])
{
	// Port argument required
        if (argc != 6)
	{
                std::cerr << "Usage: " << argv[0] << " <port> localhost <DB_user> <DB_password> <DB_name>\n";
		return -1;
	}

	// Get port number (passed as argument)
	int port = std::stoi(argv[1]);
        std::string db_server_ip(argv[2]);
        std::string db_user(argv[3]);
        std::string db_password(argv[4]);
        std::string db_name(argv[5]);

	Server server(port);

	int ret = server.createSocket();
	if(ret == -1)
	{
		std::cout << "Error on create socket\n";
		return -1;
	}

	ret = server.bindSocketToAddress();
	if(ret == -1)
	{
		std::cout << "Error on bind\n";
		return -1;
	}

	ret = server.listenOnSocket();
	if(ret == -1)
	{
		std::cout << "Error on listen\n";
		return -1;
	}

        DBParams db_params;
        db_params.ip = db_server_ip;
        db_params.user = db_user;
        db_params.password = db_password;
        db_params.db_name = db_name;

	// Serve clients (infinite loop)
        server.run(db_params);

	return 0; 
}
