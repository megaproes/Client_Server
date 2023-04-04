#include "Server.h"
#include <iostream>

Server::Server(int port) : m_port(port), m_listenSocket(INVALID_SOCKET)
{
	Command *sendMessageCmd = new SendMessageCommand("");
	addCommand("send", sendMessageCmd);

	Command *disconnectCmd = new DisconnectCommand();
	addCommand("disconnect", disconnectCmd);
}
Server::~Server() { Stop(); }

// запускает сервер и начинает слушать запросы от клиента
void Server::Start()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		return;
	}
	addrinfo hints = {}, *result = nullptr;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(nullptr, std::to_string(m_port).c_str(), &hints, &result);
	if (iResult != 0)
	{
		std::cerr << "getaddrinfo failed" << std::endl;
		WSACleanup();
		return;
	}

	m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_listenSocket == INVALID_SOCKET)
	{
		std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult)
	{
		std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(m_listenSocket);
		WSACleanup();
		return;
	}
	freeaddrinfo(result);

	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
		closesocket(m_listenSocket);
		WSACleanup();
		return;
	}
	std::cout << "Server started on port: " << m_port << std::endl;

	while (true)
	{
		SOCKET clientSocket = accept(m_listenSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
			closesocket(m_listenSocket);
			WSACleanup();
			return;
		}
		std::cout << "New client connected" << std::endl;
		ProcessClientConnection(clientSocket);
	}
}

void Server::Stop()
{
	if (m_listenSocket != INVALID_SOCKET)
	{
		closesocket(m_listenSocket);
		m_listenSocket == INVALID_SOCKET;
		std::cout << "Server stopped." << std::endl;
	}
	WSACleanup();
}

void Server::addCommand(const std::string &commandName, Command *command)
{
	m_commands.push_back(std::make_pair(commandName, command));
}

// Обрабатывает полученную команду от клиента
void Server::ProcessClientConnection(SOCKET ClientSocket)
{
	char buffer[1024] = {};
	int iResult;

	while (true)
	{
		iResult = recv(ClientSocket, buffer, sizeof(buffer), 0);
		if (iResult > 0)
		{
			std::string commandString(buffer, iResult);
			size_t spaceIndex = commandString.find(' ');
			std::string commandName = commandString.substr(0, spaceIndex);
			std::string args = commandString.substr(spaceIndex + 1);

			ProccessCommand(commandName, args);
		}
		else if (iResult == 0)
		{
			std::cout << "Client disconnected." << std::endl;
			break;
		}
		else
		{
			std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
			break;
		}
	}
	closesocket(ClientSocket);
}

void Server::ProccessCommand(const std::string &commandName, const std::string &args)
{
	for (auto &commandPair : m_commands)
	{
		if (commandPair.first == commandName)
		{
			commandPair.second->setMessage(args);
			commandPair.second->Execute();
			return;
		}
	}
	std::cerr << "Unknown command: " << commandName << std::endl;
}
