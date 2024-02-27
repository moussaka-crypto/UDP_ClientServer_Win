#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <ctime>
#include <filesystem>
#pragma comment (lib, "ws2_32.lib")

std::string get_timestamp();
void receiveMessages(SOCKET in, sockaddr_in& clientMeta, int& clientLength, std::ofstream& serverLog);
#endif