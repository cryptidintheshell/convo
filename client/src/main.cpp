// #include "../headers/client.h"
#include "../headers/window.h"
#include "../headers/messenger.h"

int main() {
	initscr();
	noecho();  // dont display typed char
	curs_set(0); // dont display cursor
	nodelay(stdscr, TRUE); // make getch non blocking
	clear(); refresh();

	// handle user creation
	WinBox boxCreateUser(50, 20, 0, 0, true, "Create user");
	WINDOW *boxCreateUserHandler = boxCreateUser.GetHandle();
	boxCreateUser.Focus();

	mvwprintw(boxCreateUserHandler, 2, 2, "Username> ");
	wrefresh(boxCreateUserHandler);

	std::string txt = boxCreateUser.GetInput();
	mvwprintw(boxCreateUserHandler, 2, 2, "Username: %s", txt.c_str());
	wrefresh(boxCreateUserHandler);

	int height, width;
	int logHeight = 10;
	getmaxyx(stdscr, height, width);

	// handle retriving list of server
	WinBox boxServerList(width, height-logHeight, 0, 0, true, "Server list");
	WINDOW *boxServerListHandler = boxServerList.GetHandle();
	boxServerList.Focus();
	wrefresh(boxServerListHandler);

	// logs window
	WinBox boxLogs(width, logHeight, 0, height-logHeight, true, "Logs");
	WINDOW *boxLogsHandler = boxLogs.GetHandle();
	wrefresh(boxLogsHandler);

	

	// handle server connection (connecting and retrieving server list)
	Messenger msger;
	if (!msger.SetupSocket(msger.mainClientSocket, msger.mainServerAddr, msger.MAIN_SERVER_PORT)) {
		boxLogs.Print("Socket setup failed.");
		wgetch(boxLogsHandler);
		exit(1);
	}

	if (!msger.ConnectToTheServer(msger.mainClientSocket, msger.mainServerAddr)) {
		boxLogs.Print("Failed to connect to the server.");
		exit(1);
	} else {
		msger.SendToServer(msger.mainClientSocket, "username." + txt);

		for (;;) {
			if (msger.ReceiveFromServer(msger.mainClientSocket) == "username.ok")
				break;
		}

		boxLogs.Print("[+] Connected to the server");
	} 

	// req server list
	boxLogs.Print("[~] Requesting server list");
	int isServerListReceived = 0;
	std::atomic<bool> stopServerListRequest(false);
	std::future<std::string> serverListResult = std::async(std::launch::async, [&msger, &stopServerListRequest](){
		return msger.GetServerList(stopServerListRequest);
	});



	// infinite loop
	for (;;) {
		char c = getch();
		if (c == 'q') {
			if (stopServerListRequest) stopServerListRequest = true;
			close(msger.mainClientSocket);
			break;
		}

		if (isServerListReceived == 0) { 
		    if (serverListResult.valid() && 
		        serverListResult.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {

		        try {
		            std::string servers = serverListResult.get();
		            if (servers == "ok") {
		                boxLogs.Print("[+] Successfully received server list");
		                isServerListReceived = 1;
		            } else {
		                boxLogs.Print("[!] Failed to receive server list: " + servers);
		                isServerListReceived = -1;
		            }

		        } catch (const std::exception& e) {
		            boxLogs.Print("[!] Thread Error: " + std::string(e.what()));
		            isServerListReceived = -1;
		        }
		    }
		}
	}

	clear(); endwin();
	return 0;
}