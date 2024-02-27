#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <chrono>
#pragma comment (lib, "ws2_32.lib")

void receiveMessages(SOCKET out);

#endif
