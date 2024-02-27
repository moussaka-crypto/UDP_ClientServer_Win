# UDP Client-Server Implementation

This project implements a simple **UDP Client-Server** communication system using _C++ and Winsock_.

## Features
- **UDP Communication**: The communication between the client and server is done using UDP (User Datagram Protocol), providing a lightweight and connectionless communication mechanism.

- **Threading**: Both the client and server utilize multithreading to handle sending and receiving messages concurrently. This allows for simultaneous communication without blocking.

- **Logging**: The server logs all received messages, including timestamps and client IP addresses, to a log file located in the `logs` directory. This provides a record of communication for debugging and analysis purposes.

- **Timestamping**: The system includes a function to generate timestamps in the format "dd-mm-yyyy_HH-MM-SS", which are used for logging purposes.

## Usage
**Server**: It will listen for incoming UDP messages on port 54000 and log them to a file.
**Client**: It sends messages to the server on localhost (127.0.0.1) port 54000.

## Build Instructions
- The project is developed using Visual Studio with the Winsock library linked (`ws2_32.lib`). You can compile the code using Visual Studio or any other IDE supporting C++ with Winsock (justdo it in separate projects).

## Dependencies
- **WS2tcpip.h**: Header file providing Winsock API for Windows Sockets programming.
- **filesystem**: C++17 feature used for creating directories to store log files.
- **thread**, **mutex**: C++ standard library features for multithreading support.

## Notes
- This implementation is intended for educational purposes and may not be suitable for production environments without further enhancements and security considerations. (I will get to it eventually.)
- Network programming is definitely more interesting than expected...
