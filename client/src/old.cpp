#include "../headers/client.h"
#include "../headers/window.h"

int main() {


	std::string name;
	std::cout << "Enter your name: ";
	std::getline(std::cin, name);


	initscr();
	noecho();  // dont display typed char
	curs_set(0); // dont display cursor
	// nodelay(stdscr, TRUE); // make getch non blocking
	clear(); refresh();

	WinBox box1(20, 10, 0, 0, true, "window 1");
	getch();


	SocketClient game1;
	game1.clientName = name;

	// connect to main port to retrieve server port lists
	game1.SetupSocket(game1.mainClientSocket, game1.mainServerAddr, MAIN_SERVER_PORT);
	game1.ConnectToTheServer(game1.mainClientSocket, game1.mainServerAddr);

    // send username to server
    std::string usernameMsg = "username." + name;
    game1.SendToServer(game1.mainClientSocket, usernameMsg.c_str());
    while (1) {
		std::string reply = game1.ReceiveFromServer(game1.mainClientSocket);
		if (reply.find("username.ok") != std::string::npos) break;
    }


	// request server list
    std::vector<int> serverPortList;
	game1.SendToServer(game1.mainClientSocket, "servers.username." + name);
    while (1) {
		std::string reply = game1.ReceiveFromServer(game1.mainClientSocket);

		if (reply.find("servers.") != std::string::npos) {
			int server1, server2, server3;
			sscanf(reply.c_str(), "servers.%i,%i,%i", &server1, &server2, &server3);
			serverPortList.push_back(server1);
			serverPortList.push_back(server2);
			serverPortList.push_back(server3);

			std::cout << "Server list retrieved.\n";
			break;
		}
    }

	// main loop
	while(1) {
		// listen to server
		std::string msg = game1.ReceiveFromServer(game1.mainClientSocket);
		if (msg.length() > 0) {
			printw(msg.c_str());
			refresh();
		}
	};

	return 0;
}