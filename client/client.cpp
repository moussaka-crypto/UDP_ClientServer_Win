#include "client.h"

void receiveMessages(SOCKET out)
{
	char buffer[1024];
	int bytesReceived = 0;
	while (true) {
		bytesReceived = recv(out, buffer, 1024, 0);
		if (bytesReceived > 0) {
			std::cout << "\t\t\t\tServer: " << buffer << "\n";
		}
	}
}