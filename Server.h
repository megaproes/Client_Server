#pragma once
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include "Command.h"
class Server
{
private:
	void ProccessCommand(const std::string &commandName, const std::string &args);
	void ProcessClientConnection(SOCKET ClientSocket);
	int m_port;
	SOCKET m_listenSocket;
	std::vector<std::pair<std::string, Command *>> m_commands;

public:
	Server(int port);
	~Server();

	void Start();
	void Stop();

	void addCommand(const std::string &commandName, Command *command);
};
