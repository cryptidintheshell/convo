#ifndef CLIENT_H
#define CLIENT_H
#include "includes.h"


class SocketClient {
    sockaddr_in serverAddr{};

public:
    const int MAIN_SERVER_PORT = 6872;
    const int BUFFER_SIZE = 1024;

    int clientSocket = -1;
    int mainClientSocket = -1;
    bool isConnected = false;
    sockaddr_in mainServerAddr{};

    ~SocketClient();
    bool SetupSocket(int& socketFd, sockaddr_in& serverAddr, int port);

    // main socket functions
    bool ConnectToTheServer(int& clientSocket, const sockaddr_in& serverInfo);
    std::string ReceiveFromServer(int& socketFd);
    bool SendToServer(int& socketFd, const std::string& msg);
};

#endif
