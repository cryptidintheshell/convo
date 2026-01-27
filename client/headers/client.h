#ifndef CLIENT_H
#define CLIENT_H

#include "includes.h"

#define MAIN_SERVER_PORT 6872
#define BUFFER_SIZE 1024

class SocketClient {
    sockaddr_in serverAddr{};

public:
    int clientSocket = -1;
    int mainClientSocket = -1;
    bool isConnected = false;

    std::string clientName = "";

    sockaddr_in mainServerAddr{};

    ~SocketClient();
    void SetupSocket(int& socketFd, sockaddr_in& serverAddr, int port);


    bool ConnectToTheServer(int& clientSocket, const sockaddr_in& serverInfo);
    void ConnectToMainServer();
    std::string ReceiveFromServer(int& socketFd);
    void SendToServer(int& socketFd, const std::string& msg);

    // retrieve server list
    std::vector<std::string> GetServerList();
};

#endif
