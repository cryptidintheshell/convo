#include "../headers/socket.h"

SocketServer::~SocketServer() {
    Warning("Closing server sockets.");
    for (auto& s : servers) {
        close(s->socketFd);
    }
}

void SocketServer::CreateServers(int count) {
    srand(time(nullptr));

    for (int i = 0; i < count; i++) {
        int port;
        do {
            port = rand() % (9999 - 4000 + 1) + 4000;
        } while (port == MAIN_SERVER_PORT);

        serverPortList.push_back(std::to_string(port));
        servers.emplace_back(std::make_unique<ServerSocket>(port));

        Notice("Created server on port " + std::to_string(port));
    }

    for (auto& server : servers) {
        SetupServer(*server);
    }
}

void SocketServer::SetupServer(ServerSocket& server) {
    server.socketFd = socket(AF_INET, SOCK_STREAM, 0);

    server.serverAddr.sin_family = AF_INET;
    server.serverAddr.sin_port = htons(server.port);
    server.serverAddr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(server.socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server.socketFd,
             (sockaddr*)&server.serverAddr,
             sizeof(server.serverAddr)) < 0) {
        perror("bind");
        return;
    }

    if (listen(server.socketFd, SOMAXCONN) < 0) {
        perror("listen");
        return;
    }

    Notice("Listening on port " + std::to_string(server.port));

    std::thread([this, &server]() {
        AcceptClients(server);
    }).detach();
}

void SocketServer::AcceptClients(ServerSocket& server) {
    while (true) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);

        int clientFd = accept(server.socketFd, (sockaddr*)&clientAddr, &len);
         if (clientFd < 0) continue;

        char ipBuf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuf, sizeof(ipBuf));

        {
            std::lock_guard<std::mutex> lock(server.clientMutex);

            int index = server.clientSockets.size();
            server.clientSockets.push_back(clientFd);
            server.clientIPs.emplace_back(ipBuf);

            Notice("[" + std::to_string(server.port) + "]: Client connected from " + server.clientIPs.back());

            std::thread([this, &server, index]() {
                HandleClient(server, index);
            }).detach();
        }
    }
}

void SocketServer::HandleClient(ServerSocket& server, int index) {
    char buffer[256];
    int currentSocket = server.clientSockets[index];

    while (true) {
        int bytes = recv(server.clientSockets[index], buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';
        std::string message = std::string(buffer);
        Notice("Received: " + message);

        std::string reply = "";

        if (message.find("servers.") != std::string::npos) {
            reply += "servers.";

            int serverPortCount = serverPortList.size();
            for (int i = 0; i < serverPortCount; i++) {
                reply += serverPortList[i]; 
                if (i == serverPortCount-1) reply += "";
                else reply += ",";
            }

            Notice("Sending: " + reply);            
            send(currentSocket, reply.c_str(), reply.length(), 0);
            Notice("Server list sent to " + server.clientNames[currentSocket]);

        } else if (message.find("username.") != std::string::npos) {
            std::string name = message.substr(9);
            server.clientNames[currentSocket] = name;
            reply = "username.ok";
            send(currentSocket, reply.c_str(), reply.length(), 0);
        }


        // clear reply
        reply = "";
    }


    { // remove disconnected client
        std::lock_guard<std::mutex> lock(server.clientMutex);

        Notice("Client disconnected: " + server.clientIPs[index]);

        close(server.clientSockets[index]);
        server.clientSockets.erase(server.clientSockets.begin() + index);
        server.clientIPs.erase(server.clientIPs.begin() + index);
    }
}

void SocketServer::Notice(const std::string& msg) {
    std::cout << "[+] " << msg << "\n";
}

void SocketServer::Warning(const std::string& msg) {
    std::cout << "[!] " << msg << "\n";
}

void SocketServer::Error(const std::string& msg) {
    std::cerr << "[!!] " << msg << "\n";
    exit(EXIT_FAILURE);
}