#ifndef UNO_NETWORK_H_INCLUDED
#define UNO_NETWORK_H_INCLUDED

#include <iostream>

//#include <WinSock2.h>
//#include <WS2tcpip.h>
#include <system_error>
#include <string>
//#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <utility>
#include <winsock2.h>
#include <stdio.h>
#include <sstream>

using namespace std;

#define PORT 2020
#define ID "almafa0000"

bool connect_type();

void connect_msg(char* connect_msg);

void rst_recvbuff(char* recvbuff);

bool server_accept_ok(char* recvbuff);

void get_command(char* command, char* recvbuff);

void get_command_yt(char* command, char* recvbuff);

int server_udp();

int client_tcp();

SOCKET client_tcp_create_socket();

int client_tcp_connect(SOCKET socket);

void client_tcp_send_string(SOCKET socket, string sendbuff);

void client_tcp_send_char(SOCKET socket, char* sendbuff);

int client_tcp_recv_char(SOCKET socket, char* recvbuff);

int client_tcp_close_socket(SOCKET socket);

void clear_recvbuf(char* recvbuf) ;



#endif // UNO_NETWORK_H_INCLUDED
