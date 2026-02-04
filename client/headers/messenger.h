#include "client.h"

class Messenger : public SocketClient {

public:
	int GetServerList();
	void PrintServerListMenu(std::vector<std::string> serverList, int currentSelected);
};