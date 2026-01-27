#ifndef SOCKET_H
#define SOCKET_H

#include "includes.h"

#define MAIN_SERVER_PORT 6872

struct ServerSocket {
    int socketFd = 0;
    int port = 0;
    sockaddr_in serverAddr{};

    std::vector<int> clientSockets;
    std::vector<std::string> clientIPs;
    std::mutex clientMutex;

    std::map<int, std::string> clientNames;

    explicit ServerSocket(int p) : port(p) {}
};


class SocketServer {
public:
    ~SocketServer();

    void CreateServers(int count);
    void SetupServer(ServerSocket& server);

private:
    std::vector<std::unique_ptr<ServerSocket>> servers;
    std::vector<std::string> serverPortList;

    void AcceptClients(ServerSocket& server);
    void HandleClient(ServerSocket& server, int clientIndex);

    void Notice(const std::string& msg);
    void Warning(const std::string& msg);
    void Error(const std::string& msg);
};

#endif