#include <iostream>
#include <sstream>

#include "Client.h"
using namespace std;

int main()
{
	Client client("127.0.0.1", 8080);
	client.Connect();
	std::string request(1024, 0);

	while (true)
	{
		request.clear();
		std::cout << "User: " << std::endl;
		getline(std::cin, request);
		client.SendCommand("send", request);
	}
	client.Disconnect();
	return 0;
}