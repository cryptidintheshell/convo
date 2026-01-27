#include "../headers/socket.h"

int main() {
	SocketServer sock1;

	// create main socket for client's server list request
	ServerSocket mainServerSocket(MAIN_SERVER_PORT);
	sock1.SetupServer(mainServerSocket);

	sock1.CreateServers(3);
	while(1) {};

	return 0;
}