#include "client.h"

class Messenger : public SocketClient {

public:
	std::string GetServerList(std::atomic<bool> &stopFlag);
	// void PrintServerListMenu(std::vector<std::string> serverList, int currentSelected);
};