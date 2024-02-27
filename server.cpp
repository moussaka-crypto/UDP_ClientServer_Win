#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <ctime>
#include <filesystem>
#pragma comment (lib, "ws2_32.lib")

std::mutex mtx;
std::string get_timestamp() {
	std::time_t now = std::time(nullptr);
	// Convert to local time!!
	std::tm localTime;
	localtime_s(&localTime, &now);

	// Format as a string
	char timeStamp[20];
	std::strftime(timeStamp, sizeof(timeStamp), "%d-%m-%Y_%H-%M-%S", &localTime);

	return timeStamp;
}

void receiveMessages(SOCKET in, sockaddr_in& clientMeta, int& clientLength, std::ofstream& serverLog)
{
	char buff[1024];
	char clientIP[256];

	while (true)
	{
		ZeroMemory(buff, 1024); // zero in every time we receive to avoid junk
		int bytesIn = recvfrom(in, buff, 1024, 0, (sockaddr*)&clientMeta, &clientLength);
		ZeroMemory(clientIP, 256);

		if (bytesIn == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			switch (errorCode)
			{
			case WSAECONNRESET: // Connection reset by peer
				std::cout << "Client disconnected unexpectedly.\n";
				break;
			case WSAEMSGSIZE: // too large
				std::cout << "Received message exceeds maximum size.\n";
				break;
			default:
				std::cerr << "Error receiving from client: " << errorCode << '\n';
			}
		}
		else if (bytesIn > 0)
		{
			buff[bytesIn] = '0';
			// convert to printable form
			inet_ntop(AF_INET, &clientMeta.sin_addr, clientIP, 256);
			// Lock the mutex before writing to the log file
			std::lock_guard<std::mutex> lock(mtx);
			serverLog << "[" << get_timestamp() << "] Client(" << clientIP << "): " << buff << "\n";
			serverLog.flush();
		}
		std::cout << "\t\t\t\tClient(" << clientIP << "): " << buff << "\n";
	}
	serverLog.close(); //end of conversation
}

void main()
{
	//Startup Winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);

	if (wsOK != 0) {
		std::cerr << "Error: Unable to start Winsock, error code: " << wsOK << '\n';
		return;
	}

	// AF is adress family
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
	if (in == INVALID_SOCKET)
	{
		std::cout << "Error creating socket: " << WSAGetLastError() << '\n';
		WSACleanup();
	}

	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // give me any address, whatever address my nic is using
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000); // host to network short - little to big endian
	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		std::cout << "Can't bind socket!" << WSAGetLastError() << '\n';
		closesocket(in);
		WSACleanup();
		return;
	}

	std::filesystem::create_directory("logs");
	std::string logFileName = "logs/log_" + get_timestamp() + ".txt";
	std::ofstream serverLog(logFileName);
	if (!serverLog.is_open())
	{
		std::cerr << "Error opening log file: " << logFileName << std::endl;
		// maybe close socket later
		return;
	}

	// Client metadata
	sockaddr_in clientMeta;
	int clientLength = sizeof(clientMeta);
	ZeroMemory(&clientMeta, clientLength);

	// std::ref for copyable or movable arguments
	std::thread receiver(receiveMessages, in, std::ref(clientMeta),
		std::ref(clientLength),
		std::ref(serverLog));
	std::cout << "///////////SERVER////////////\n\n";
	while (true)
	{
		std::string serverMsg;
		std::getline(std::cin, serverMsg);
		std::cout.flush();

		mtx.lock();
		int sendResult = sendto(in, serverMsg.c_str(), serverMsg.size() + 1, 0,
			(sockaddr*)&clientMeta, clientLength);
		mtx.unlock();

		if (sendResult == SOCKET_ERROR) {
			std::cout << "Error sending to client: " << WSAGetLastError() << '\n';
		}
		else if (sendResult > 0)
		{
			std::lock_guard<std::mutex> lock(mtx);
			serverLog << "[" << get_timestamp() << "] Server: " << serverMsg << '\n';
			serverLog.flush();
		}
	}

	//Close socket and shutdown winsock
	serverLog.close();
	closesocket(in);
	WSACleanup();
}