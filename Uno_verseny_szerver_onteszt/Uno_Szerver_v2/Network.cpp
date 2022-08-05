#include "Network.h"

using namespace std;

int kliens_udp() {

     WSADATA              wsaData;
     SOCKET               SendingSocket;
     SOCKADDR_IN          ReceiverAddr, SrcInfo;
     int                  Port = 2010;
     char                 *SendBuf = "Sending all my love, Sending all my love to youuu!";
     int                  BufLength = 1024;

     // Initialize Winsock version 2.2
     if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
          printf("Client: WSAStartup failed with error %d\n", WSAGetLastError());

          // Clean up
          WSACleanup();

          // Exit with error
          return -1;
     }
     ///else
          ///printf("Client: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);

     // Create a new socket to receive datagrams on.
     SendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
     if (SendingSocket == INVALID_SOCKET){
          printf("Client: Error at socket(): %d\n", WSAGetLastError());

          // Clean up
          WSACleanup();

          // Exit with error
          return -1;
     }
     ///else
          ///printf("Client: socket() is OK!\n");

     // Set up a SOCKADDR_IN structure that will identify who we
     // will send datagrams to. For demonstration purposes, let's
     // assume our receiver's IP address is 127.0.0.1 and waiting
     // for datagrams on port 2010.
     ReceiverAddr.sin_family = AF_INET;
     ReceiverAddr.sin_port = htons(Port);
     ReceiverAddr.sin_addr.s_addr = inet_addr("81.183.153.27");

     // Send a datagram to the receiver.
     ///printf("Client: Data to be sent: \"%s\"\n", SendBuf);
     ///printf("Client: Sending datagrams...\n");
     int TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0,(SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr));

     ///printf("Client: sendto() looks OK!\n");

     // Some info on the receiver side...
     // Allocate the required resources
     memset(&SrcInfo, 0, sizeof(SrcInfo));
     int len = sizeof(SrcInfo);

     //getsockname(SendingSocket, (SOCKADDR *)&SrcInfo, &len);
     ///printf("Client: Sending IP(s) used: %s\n", inet_ntoa(SrcInfo.sin_addr));
     ///printf("Client: Sending port used: %d\n", htons(SrcInfo.sin_port));

     // Some info on the sender side
     //getpeername(SendingSocket, (SOCKADDR *)&ReceiverAddr, (int *)sizeof(ReceiverAddr));

     ///printf("Client: Receiving IP used: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
     ///printf("Client: Receiving port used: %d\n", htons(ReceiverAddr.sin_port));
     ///printf("Client: Total byte sent: %d\n", TotalByteSent);

   // When your application is finished receiving datagrams close the socket.
   ///printf("Client: Finished sending. Closing the sending socket...\n");

   if (closesocket(SendingSocket) != 0)
        printf("Client: closesocket() failed! Error code: %d\n", WSAGetLastError());
   ///else
        ///printf("Server: closesocket() is OK\n");

   // When your application is finished call WSACleanup.
   ///printf("Client: Cleaning up...\n");
   if(WSACleanup() != 0)
        printf("Client: WSACleanup() failed! Error code: %d\n", WSAGetLastError());
   ///else
        ///printf("Client: WSACleanup() is OK\n");

   // Back to the system
   return 0;

}

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

int server_tcp(){

     WSADATA            wsaData;
     SOCKET             ListeningSocket, NewConnection;
     SOCKADDR_IN        ServerAddr, SenderInfo;
     int                Port = PORT;

     // Receiving part
     char          recvbuff[1024];
     int                ByteReceived, nlen, SelectTiming;

     // Initialize Winsock version 2.2
     if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {

          // The WSAGetLastError() function is one of the only functions
          // in the Winsock 2.2 DLL that can be called in the case of a WSAStartup failure
          printf("Server: WSAStartup failed with error %d.\n", WSAGetLastError());

          // Exit with error
          return 1;
     } else {
          ///printf("Server: The Winsock DLL found!\n");
          ///printf("Server: The current status is %s.\n", wsaData.szSystemStatus);
     }

     if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 ) {

          //Tell the user that we could not find a usable WinSock DLL
          printf("Server: The dll do not support the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion),HIBYTE(wsaData.wVersion));

          // Do the clean up
          WSACleanup();

          // and exit with error
          return 1;
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
          WSACleanup();

          // and exit with error
          return 1;
     }
     ///else
          ///printf("Server: socket() is OK!\n");


     // Set up a SOCKADDR_IN structure that will tell bind that we
     // want to listen for connections on all interfaces using port 2010.
     // The IPv4 family
     ServerAddr.sin_family = AF_INET;

     // host-to-network byte order
     ServerAddr.sin_port = htons(Port);

     // Listen on all interface, host-to-network byte order
     ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);


     // Associate the address information with the socket using bind.
     // Call the bind function, passing the created socket and the sockaddr_in
     // structure as parameters. Check for general errors.
     if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR) {

          printf("Server: bind() failed! Error code: %d.\n", WSAGetLastError());

          // Close the socket
          closesocket(ListeningSocket);

          // Do the clean up
          WSACleanup();

          // and exit with error
          return 1;
     }
     ///else
          ///printf("Server: bind() is OK!\n");


     // Listen for client connections with a backlog of 5
     if (listen(ListeningSocket, 5) == SOCKET_ERROR) {
          printf("Server: listen(): Error listening on socket %d.\n", WSAGetLastError());

          // Close the socket
          closesocket(ListeningSocket);

          // Do the clean up
          WSACleanup();

          // Exit with error
          return 1;
     }
     ///else
          ///printf("Server: listen() is OK, I'm listening for connections...\n");



     // Set 10 seconds 10 useconds timeout
     SelectTiming = recvTimeOutTCP(ListeningSocket, 10, 10);

     switch (SelectTiming) {

          case 0:
              // Timed out, do whatever you want to handle this situation
               printf("\nServer: Timeout lor while waiting you retard client!...\n");
              break;

          case -1:
              // Error occurred, more tweaking here and the recvTimeOutTCP()...
              printf("\nServer: Some error encountered with code number: %d\n", WSAGetLastError());
              break;

          default:
              {

                   // Accept a new connection when available. 'while' always true
                   while(1)
                   {
                        // Reset the NewConnection socket to SOCKET_ERROR
                        // Take note that the NewConnection socket in not listening
                        NewConnection = SOCKET_ERROR;
                        // While the NewConnection socket equal to SOCKET_ERROR
                        // which is always true in this case...
                        while(NewConnection == SOCKET_ERROR)
                        {
                             // Accept connection on the ListeningSocket socket and assign
                             // it to the NewConnection socket, let the ListeningSocket
                             // do the listening for more connection
                             NewConnection = accept(ListeningSocket, NULL, NULL);
                             closesocket(ListeningSocket);
                             ///printf("\nServer: accept() is OK...\n");
                             ///printf("Server: New client got connected, ready to receive and send data...\n");

                             // At this point you can do two things with these sockets
                             // 1. Wait for more connections by calling accept again
                             //    on ListeningSocket (loop)
                             // 2. Start sending or receiving data on NewConnection.
                             int cnt=0;
                             int t=time(NULL);
                             while (true){
                                 ByteReceived = recv(NewConnection, recvbuff, sizeof(recvbuff), 0);
                                 //cout<<ByteReceived << " " << recvbuff<<endl;
                                 for (int i=0; i<1024; i++) recvbuff[i]=0;

                                 //ByteReceived = recv(NewConnection, recvbuff, sizeof(recvbuff), 0);
                                 //cout<<ByteReceived << " " << recvbuff<<endl;
                                 //for (int i=0; i<1024; i++) recvbuff[i]=0;

                                 ByteReceived = send(NewConnection, "alma1", sizeof("alma1"), 0);
                                 ///cout<<ByteReceived << " " << recvbuff<<endl;

                                 //ByteReceived = send(NewConnection, "korte2", sizeof("korte2"), 0);
                                 ///cout<<ByteReceived << " " << recvbuff<<endl;

                                 /*ByteReceived = recv(NewConnection, recvbuff, sizeof(recvbuff), 0);
                                 ///cout<<ByteReceived << " " << recvbuff<<endl;
                                 ByteReceived = send(NewConnection, "barack3", sizeof("barack3"), 0);
                                 ///cout<<ByteReceived << " " << recvbuff<<endl;*/

                                 cnt++;
                                 if (t<time(NULL)){
                                    t=time(NULL);
                                    cout<<"FPS: "<<cnt<<endl;
                                    cnt=0;
                                 }
                             }

                             // When there is data
                             if ( ByteReceived > 0 ) {
                                  ///printf("Server: recv() looks fine....\n");

                                  // Some info on the receiver side...
                                  getsockname(ListeningSocket, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));
                                  ///printf("Server: Receiving IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
                                  ///printf("Server: Receiving port used: %d\n", htons(ServerAddr.sin_port));

                                  // Some info on the sender side
                                  // Allocate the required resources
                                  memset(&SenderInfo, 0, sizeof(SenderInfo));
                                  nlen = sizeof(SenderInfo);

                                  getpeername(NewConnection, (SOCKADDR *)&SenderInfo, &nlen);
                                  ///printf("Server: Sending IP used: %s\n", inet_ntoa(SenderInfo.sin_addr));
                                  ///printf("Server: Sending port used: %d\n", htons(SenderInfo.sin_port));

                                  // Print the received bytes. Take note that this is the total
                                  // byte received, it is not the size of the declared buffer
                                  ///printf("Server: Bytes received: %d\n", ByteReceived);

                                  // Print what those bytes represent
                                  ///printf("Server: Those bytes are: ");

                                  // Print the string only, discard other
                                  // remaining 'rubbish' in the 1024 buffer size
                                 ///for(i=0;i < ByteReceived;i++)
                                      ///printf("%c", recvbuff[i]);
                                  ///printf("\n");
                             }
                             // No data
                             else if ( ByteReceived == 0 )
                                  printf("Server: Connection closed!\n");
                             // Others
                             else
                                  printf("Server: recv() failed with error code: %d\n", WSAGetLastError());
                        }

                        // Clean up all the send/recv communication, get ready for new one
                        if( shutdown(NewConnection, SD_SEND) != 0)
                             printf("\nServer: Well, there is something wrong with the shutdown(). The error code: %d\n", WSAGetLastError());
                        else
                             printf("\nServer: shutdown() looks OK...\n");

                        // Well, if there is no more connection in 15 seconds,
                        // just exit this listening loop...
                        if( recvTimeOutTCP(ListeningSocket, 60, 60) == 0)
                             break;
                   }
              }
     }

     printf("\nServer: The listening socket is timeout...\n");

     // When all the data communication and listening finished, close the socket
     if(closesocket(ListeningSocket) != 0)
          printf("Server: Cannot close ListeningSocket socket. Error code: %d\n", WSAGetLastError());
     else
          printf("Server: Closing ListeningSocket socket...\n");

     // Finally and optionally, clean up all those WSA setup
     if(WSACleanup() != 0)
          printf("Server: WSACleanup() failed! Error code: %d\n", WSAGetLastError());
     else
          printf("Server: WSACleanup() is OK...\n");

     return 0;
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

int server_tcp_bind(SOCKET ListeningSocket, u_short port) {
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
        return 1;
    }
    ///else
        ///printf("Server: bind() is OK!\n");
}

int server_tcp_start_listening(SOCKET ListeningSocket) {

    if (listen(ListeningSocket, 5) == SOCKET_ERROR) {
          printf("Server: listen(): Error listening on socket %d.\n", WSAGetLastError());

          // Close the socket
          closesocket(ListeningSocket);

          // Do the clean up
          ///WSACleanup();

          // Exit with error
          return -1;
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
    while(NewConnection == SOCKET_ERROR) {
        if (t+10<time(NULL))
            return SOCKET_ERROR;
        NewConnection = accept(ListeningSocket, NULL, NULL);
        closesocket(ListeningSocket);
        ///printf("\nServer: accept() is OK...\n");
        ///printf("Server: New client got connected, ready to receive and send data...\n");
    }

    return NewConnection;
}

int server_tcp_send_char (SOCKET Connection_socket, char* sendbuff) {
    int BytesSent = send(Connection_socket, sendbuff, strlen(sendbuff), 0);
    ///cout<<BytesSent << " " << sendbuff<<endl;
    if(BytesSent == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
}

int server_tcp_send_string (SOCKET Connection_socket, string sendbuff) {
    int BytesSent = send(Connection_socket, sendbuff.c_str(), sendbuff.size(), 0);
    ///cout<<BytesSent << " " << sendbuff<<endl;
    if(BytesSent == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
}

int server_tcp_recv_char (SOCKET Connection_socket, char* recvbuff) {
    for (int i=0; i<1024; i++) recvbuff[i]=0;
    int val = 10000;
    setsockopt(Connection_socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&val,sizeof(int));
    int BytesReceived = recv(Connection_socket, recvbuff, 1024, 0);
    ///cout<<BytesReceived << " " << recvbuff<<endl;
    if(BytesReceived == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
}

void server_tcp_shutdown(SOCKET NewConnection) {
    // Clean up all the send/recv communication, get ready for new one
    if( shutdown(NewConnection, SD_SEND) != 0)
         printf("\nServer: Well, there is something wrong with the shutdown(). The error code: %d\n", WSAGetLastError());
    else
         printf("\nServer: shutdown() looks OK...\n");
}

