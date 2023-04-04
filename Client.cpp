#include "Client.h"
#include <iostream>

Client::Client(const std::string &serverIP, int port)
    : m_socket(INVALID_SOCKET), m_serverAddr(nullptr)
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		return;
	}
	addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(serverIP.c_str(), std::to_string(port).c_str(), &hints, &m_serverAddr);
	if (iResult != 0)
	{
		std::cerr << "getaddrinfo failed" << std::endl;
		WSACleanup();
		return;
	}

	m_socket = socket(m_serverAddr->ai_family, m_serverAddr->ai_socktype, m_serverAddr->ai_protocol);
	if (m_socket == INVALID_SOCKET)
	{
		std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(m_serverAddr);
		WSACleanup();
		return;
	}
}
Client::~Client() { Disconnect(); }

void Client::Connect()
{
	int iResult = connect(m_socket, m_serverAddr->ai_addr, (int)m_serverAddr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(m_serverAddr);
		closesocket(m_socket);
		WSACleanup();
		return;
	}
	std::cout << "Connected to server." << std::endl;
}

void Client::Disconnect()
{
	if (m_socket != INVALID_SOCKET)
	{
		shutdown(m_socket, SD_SEND);
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		std::cout << "Disconnected from server." << std::endl;
	}
	if (m_serverAddr != nullptr)
	{
		freeaddrinfo(m_serverAddr);
		m_serverAddr = nullptr;
	}
	WSACleanup();
}

//  который отправляет команду серверу
void Client::SendCommand(const std::string &commandName, const std::string &args)
{
	std::string commandString = commandName + " " + args;
	int iResult = send(m_socket, commandString.c_str(), commandString.length(), 0);
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "send failed: " << WSAGetLastError() << std::endl;
	}
}
