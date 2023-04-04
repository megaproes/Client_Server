#include <iostream>
#include "Server.h"
using namespace std;

int main()
{
	Server server(8080);
	while (true)
	{
		server.Start();
	}
	server.Stop();
	return 0;
}