#include "Network.h"

using namespace std;

int recvTimeOutTCP(SOCKET socket, long sec, long usec) {

    // Setup timeval variable
    struct timeval timeout;
    struct fd_set fds;

    // assign the second and microsecond variables
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;

    // Setup fd_set structure
    FD_ZERO(&fds);
    FD_SET(socket, &fds);

    // Possible return values:
    // -1: error occurred
    // 0: timed out
    // > 0: data ready to be read
    return select(0, &fds, 0, 0, &timeout);
}

SOCKET server_tcp_create_listening_socket() {

     WSADATA            wsaData;
     SOCKET             ListeningSocket;

     // Receiving part

     // Initialize Winsock version 2.2
     if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {

          // The WSAGetLastError() function is one of the only functions
          // in the Winsock 2.2 DLL that can be called in the case of a WSAStartup failure
          printf("Server: WSAStartup failed with error %d.\n", WSAGetLastError());

          // Exit with error
          return INVALID_SOCKET;
     } else {
          ///printf("Server: The Winsock DLL found!\n");
          ///printf("Server: The current status is %s.\n", wsaData.szSystemStatus);
     }

     if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 ) {

          //Tell the user that we could not find a usable WinSock DLL
          printf("Server: The dll do not support the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion),HIBYTE(wsaData.wVersion));

          // Do the clean up
          ///WSACleanup();

          // and exit with error
          return INVALID_SOCKET;
     } else {
          ///printf("Server: The dll supports the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion),HIBYTE(wsaData.wVersion));
          ///printf("Server: The highest version this dll can support is %u.%u\n", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
     }

     // Create a new socket to listen for client connections.
     ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

     // Check for errors to ensure that the socket is a valid socket.
     if (ListeningSocket == INVALID_SOCKET){
          printf("Server: Error at socket(), error code: %d.\n", WSAGetLastError());

          // Clean up
          ///WSACleanup();

          // and exit with error
          return INVALID_SOCKET;
     }
     ///else
          ///printf("Server: socket() is OK!\n");

    return ListeningSocket;
}

void server_tcp_bind(SOCKET ListeningSocket, u_short port) {
    SOCKADDR_IN  ServerAddr;

    ServerAddr.sin_family = AF_INET;

    // host-to-network byte order
    ServerAddr.sin_port = htons(port);

    // Listen on all interface, host-to-network byte order
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR) {

        printf("Server: bind() failed! Error code: %d.\n", WSAGetLastError());

        // Close the socket
        closesocket(ListeningSocket);

        // Do the clean up
        ///WSACleanup();

        // and exit with error
        ///return 1;
    }
    ///else
        ///printf("Server: bind() is OK!\n");
}

void server_tcp_start_listening(SOCKET ListeningSocket) {
    if (listen(ListeningSocket, 5) == SOCKET_ERROR) {
          printf("Server: listen(): Error listening on socket %d.\n", WSAGetLastError());

          // Close the socket
          closesocket(ListeningSocket);

          // Do the clean up
          ///WSACleanup();

          // Exit with error
          ///return -1;
     }
     ///else
          ///printf("Server: listen() is OK, I'm listening for connections...\n");

}

int server_tcp_check_listening(SOCKET ListeningSocket) {
    // Set 10 seconds 10 useconds timeout
    int SelectTiming = recvTimeOutTCP(ListeningSocket, 0, 0);
    return SelectTiming;
}

SOCKET server_tcp_accept(SOCKET ListeningSocket) {

    // Reset the NewConnection socket to SOCKET_ERROR
    // Take note that the NewConnection socket in not listening
    SOCKET NewConnection = SOCKET_ERROR;
    // While the NewConnection socket equal to SOCKET_ERROR
    // which is always true in this case...
    int t=time(NULL);
    while(NewConnection == (size_t)SOCKET_ERROR) {
        if (t+10<time(NULL)){
            cout<<"server_tcp_accept(): Baj van!"<<endl;
            cout<< NewConnection << endl;
            return SOCKET_ERROR;
        }
        NewConnection = accept(ListeningSocket, NULL, NULL);
        closesocket(ListeningSocket);
        ///printf("\nServer: accept() is OK...\n");
        ///printf("Server: New client got connected, ready to receive and send data...\n");
    }

    SOCKADDR_IN ServerAddr;
    getsockname(NewConnection, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));
    printf("Client: Receiver IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
    printf("Client: Receiver port used: %d\n", htons(ServerAddr.sin_port));
    return NewConnection;
}

int server_tcp_send_char (SOCKET Connection_socket, char* sendbuff) {

    int BytesSent = send(Connection_socket, sendbuff, strlen(sendbuff), 0);
    ///cout<<BytesSent << " " << sendbuff<<endl;
    if(BytesSent == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
    return BytesSent;
}

int server_tcp_send_string (SOCKET Connection_socket, string sendbuff) {
    int BytesSent = send(Connection_socket, sendbuff.c_str(), sendbuff.size(), 0);
    ///cout<<BytesSent << " " << sendbuff<<endl;
    if(BytesSent == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
    return BytesSent;
}

int server_tcp_recv_char (SOCKET Connection_socket, char* recvbuff) {
    for (int i=0; i<1024; i++) recvbuff[i]=0;
    int val = 1000;
    setsockopt(Connection_socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&val,sizeof(int));
    int BytesReceived = recv(Connection_socket, recvbuff, 1024, 0);
    ///cout<<BytesReceived << " " << recvbuff<<endl;
    if(BytesReceived == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
    return BytesReceived;
}

void server_tcp_shutdown(SOCKET NewConnection) {
    // Clean up all the send/recv communication, get ready for new one
    if( shutdown(NewConnection, SD_SEND) != 0)
         printf("\nServer: Well, there is something wrong with the shutdown(). The error code: %d\n", WSAGetLastError());
    else
         printf("\nServer: shutdown() looks OK...\n");
}

