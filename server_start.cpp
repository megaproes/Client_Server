#include <iostream>
#include "Server.h"
using namespace std;

int main()
{
	Server server(8080);

	server.Start();

	server.Stop();
	return 0;
}