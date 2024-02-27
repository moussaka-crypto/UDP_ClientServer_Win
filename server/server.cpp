#include "server.h"

std::string get_timestamp() {
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    //converts to local time and stores it there
    localtime_s(&localTime, &now);

    char timeStamp[20];
    std::strftime(timeStamp, sizeof(timeStamp), "%d-%m-%Y_%H-%M-%S", &localTime);
    return timeStamp;
}
