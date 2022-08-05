#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <utility>
#include <list>
#include <sstream>
#include <chrono>
#include <windows.h>
#include <fstream>

#define TCP true
#define PORT 2010
//#define DEBUG_CMD
//#define DEBUG_FILE false
#define DEBUG_CMD_commands false
#define DEBUG_CMD_game_count true
#define DEBUG_CMD_game_cnt 200
#define client_cnt 18


int recvTimeOutTCP(SOCKET socket, long sec, long usec);

SOCKET server_tcp_create_listening_socket();

void server_tcp_bind(SOCKET ListeningSocket, u_short port);

void server_tcp_start_listening(SOCKET ListeningSocket);

int server_tcp_check_listening(SOCKET ListeningSocket);

SOCKET server_tcp_accept(SOCKET ListeningSocket);

int server_tcp_send_char (SOCKET Connection_socket, char* sendbuff);

int server_tcp_send_string (SOCKET Connection_socket, std::string sendbuff);

int server_tcp_recv_char (SOCKET Connection_socket, char* recvbuff);

void server_tcp_shutdown(SOCKET NewConnection);



#endif // NETWORK_H_INCLUDED
