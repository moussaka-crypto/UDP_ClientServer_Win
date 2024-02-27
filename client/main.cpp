#include "client.h"

void main(int argc, char* argv[]) // we can pass commands in (argument count and argument values)
{
	//Startup Winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);

	if (wsOK != 0) {
		std::cerr << "Error: Unable to start Winsock, error code: " << wsOK << '\n';
		return;
	}

	//Create Hint structure for server
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54000);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	//Socket creation
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
	if (out == INVALID_SOCKET)
	{
		std::cout << "Error creating socket: " << WSAGetLastError() << '\n';
		WSACleanup();
		return;
	}

	//announce presence to server
	std::string wazzupBeijing("Client connected.");
	int validHello = sendto(out, wazzupBeijing.c_str(), wazzupBeijing.size() + 1, 0,
		(sockaddr*)&server, sizeof(server));

	if (validHello == SOCKET_ERROR)
	{
		std::cout << "Error initiating connection to server: "
			<< WSAGetLastError() << '\n';
		closesocket(out);
		WSACleanup();
		return;
	}

	std::thread receiver(receiveMessages, out);
	std::cout << "///////////CLIENT////////////\n\n";
	while (true)
	{
		std::string clientMsg;
		std::getline(std::cin, clientMsg);
		std::cout.flush();

		int sendOK = sendto(out, clientMsg.c_str(), clientMsg.size() + 1, 0,
			(sockaddr*)&server, sizeof(server));
		if (sendOK == SOCKET_ERROR)
		{
			std::cout << "Error sending to server: " << WSAGetLastError() << '\n';
		}
	}

	//Close socket and shutdown winsock
	closesocket(out);
	WSACleanup();
}