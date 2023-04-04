#pragma once
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include "Command.h"
class Client
{
private:
	SOCKET m_socket;
	addrinfo *m_serverAddr;

public:
	Client(const std::string &serverIP, int port);
	~Client();

	void Connect();
	void Disconnect();

	void SendCommand(const std::string &commandName, const std::string &args);
};