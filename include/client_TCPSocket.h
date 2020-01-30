#include "TCPSocket.h"

class ClientTCPSocket : public TCPSocket {
public:
	ClientTCPSocket(int port, std::string IP);
	~ClientTCPSocket();
	int connectToServer();
private:	
	const std::string server_ip_;
};