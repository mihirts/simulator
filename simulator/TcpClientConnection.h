#ifndef TCPCLIENTCONNECTION_H
#define TCPCLIENTCONNECTION_H

#include <string>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Link with Winsock library

class TcpClientConnection {
public:
    TcpClientConnection(const std::string& serverIp, int port);
    ~TcpClientConnection();

    bool connectToServer();
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    void disconnect();

private:
    SOCKET sockfd;
    sockaddr_in serverAddr;
    bool isConnected;
};

#endif
