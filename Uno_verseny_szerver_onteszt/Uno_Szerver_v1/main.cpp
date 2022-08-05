#include <iostream>

using namespace std;

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

#include <stdio.h>

#include <winsock2.h>

#include "Network.h"
#include "Game.h"
#include "Server.h"

using namespace std;
using namespace std::chrono;

int main() {
    srand(time(NULL));

    string x="PUT LM AZ TE";
    stringstream s; s<<x;
    while (true){
        s>>x; cout<<x<<" "<<x.size()<<endl;
        if (s.eof())
            break;
    }

    vector<Client> clients;
    vector<Game> games;

    prepare(clients, games);

    while (true){

        relisten_clients(clients);

        //cout<<"accept_clients"<<endl;
        accept_clients(clients, games);
        //add_clients_to_game(clients, games);
        next_steps(clients, games);
        //actualize_clients(clients, games);
    }



    if (false) {
        SOCKET Listeningsocket_1 = server_tcp_create_listening_socket();
        server_tcp_bind(Listeningsocket_1,2021);
        SOCKET Connection_socket_1;
        server_tcp_start_listening(Listeningsocket_1);

        switch (server_tcp_check_listening(Listeningsocket_1)) {
            case 0:
                // Timed out, do whatever you want to handle this situation
                printf("\nServer: Timeout lor while waiting you retard client!...\n");
                return -2;
            case -1:
                // Error occurred, more tweaking here and the recvTimeOutTCP()...
                printf("\nServer: Some error encountered with code number: %d\n", WSAGetLastError());
                return -3;
            default:
                Connection_socket_1 = server_tcp_accept(Listeningsocket_1);
                cout<<"ok1"<<endl;
        }

        SOCKET Listeningsocket_2 = server_tcp_create_listening_socket();
        server_tcp_bind(Listeningsocket_2,2022);
        SOCKET Connection_socket_2;
        server_tcp_start_listening(Listeningsocket_2);

        switch (server_tcp_check_listening(Listeningsocket_2)) {
            case 0:
                // Timed out, do whatever you want to handle this situation
                printf("\nServer: Timeout lor while waiting you retard client!...\n");
                return -2;
            case -1:
                // Error occurred, more tweaking here and the recvTimeOutTCP()...
                printf("\nServer: Some error encountered with code number: %d\n", WSAGetLastError());
                return -3;
            default:
                Connection_socket_2 = server_tcp_accept(Listeningsocket_2);
        }


        char recvbuff[1024];
        int cnt=0, t=time(NULL);
        while (true) {
            server_tcp_send_string(Connection_socket_1, "alma1");
            server_tcp_recv_char(Connection_socket_1, recvbuff);

            server_tcp_send_string(Connection_socket_2, "barack2");
            server_tcp_recv_char(Connection_socket_2, recvbuff);
            //cout<<recvbuff;

            cnt++;
            if (t<time(NULL)){
                t=time(NULL);
                cout<<"FPS: "<<cnt<<endl;
                cnt=0;
            }
        }
    }

    if (false) {
        if (TCP){
            while (true)
                server_tcp();
        } else {
            while (true)
                kliens_udp();
        }
    }

    int i;
    cin>>i;
    cout<<i;
}
