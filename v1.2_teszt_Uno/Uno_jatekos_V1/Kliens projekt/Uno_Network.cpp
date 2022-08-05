#include "Uno_Network.h"

using namespace std;

bool connect_type() {
    printf("Botok ellen (1), vagy valódi játékosok ellen mennél(2)?  (1/2): ");
    int ans;
    scanf("%d", &ans);
    if (ans==1)
        return true;
    ///else if (ans==2)
       /// return false;
    return false;
}

void connect_msg(char* connect_msg) {
    bool type = connect_type();

    strcpy(connect_msg,"CONNECT");
    char ans[3];
    if (type){
        strcat(connect_msg," TEST ");

        printf("Hany bot legyen ellened? (1-7): ");
    } else {
        strcat(connect_msg," ROOM ");
        printf("Melyik szobaba csatlakoznal? (20-86): ");
    }
    scanf("%s", ans);
    strcat(connect_msg,ans);

    strcat(connect_msg," ");
    strcat(connect_msg,ID);
    ///strcpy(connect_msg,connect_msg);
    cout<<connect_msg<<" "<<sizeof(connect_msg)<<" "<<strlen(connect_msg)<<endl;
}

void rst_recvbuff(char* recvbuff) {
    for (int i=0; i<1024; i++) recvbuff[i]=0;
}

bool server_accept_ok(char* recvbuff) {
    if (recvbuff[0]=='O' && recvbuff[1]=='K')
        return true;
    return false;
}

void get_command(char* command, char* recvbuff) {
    stringstream s;
    s <<recvbuff;
    s >> command;
}

void get_command_yt(char* command, char* recvbuff) {
    stringstream s;
    s <<recvbuff;
    s >> command;
    //cout<<command<<endl;
    s >> command;
    //cout<<command<<endl;
}

int server_udp() {

     WSADATA            wsaData;
     SOCKET             ReceivingSocket;
     SOCKADDR_IN        ReceiverAddr;
     int                Port = 2010;
     char          ReceiveBuf[1024];
     int                BufLength = 1024;
     SOCKADDR_IN        SenderAddr;
     int                SenderAddrSize = sizeof(SenderAddr);
     int                ByteReceived = 5;

   // Initialize Winsock version 2.2
   if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
   {
        printf("Server: WSAStartup failed with error %d\n", WSAGetLastError());
        return -1;
   }
   ///else
        ///printf("Server: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);

     // Create a new socket to receive datagrams on.
     ReceivingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

     if (ReceivingSocket == INVALID_SOCKET)
     {
          printf("Server: Error at socket(): %d\n", WSAGetLastError());

          // Clean up
          WSACleanup();

          // Exit with error
          return -1;
     }
     ///else
          ///printf("Server: socket() is OK!\n");

     // Set up a SOCKADDR_IN structure that will tell bind that we

     // want to receive datagrams from all interfaces using port 5150.

     // The IPv4 family
     ReceiverAddr.sin_family = AF_INET;

     // Port no. 5150
     ReceiverAddr.sin_port = htons(Port);

     // From all interface (0.0.0.0)
     ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

   // Associate the address information with the socket using bind.
   // At this point you can receive datagrams on your bound socket.
   if (bind(ReceivingSocket, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr)) == SOCKET_ERROR)
   {
        printf("Server: bind() failed! Error: %d.\n", WSAGetLastError());

        // Close the socket
        closesocket(ReceivingSocket);

        // Do the clean up
        WSACleanup();

        // and exit with error
        return -1;
     }
     ///else
        ///printf("Server: bind() is OK!\n");

   // Some info on the receiver side...
   getsockname(ReceivingSocket, (SOCKADDR *)&ReceiverAddr, (int *)sizeof(ReceiverAddr));

   ///printf("Server: Receiving IP(s) used: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
   ///printf("Server: Receiving port used: %d\n", htons(ReceiverAddr.sin_port));
   ///printf("Server: I\'m ready to receive a datagram...\n");

   // At this point you can receive datagrams on your bound socket.
   ByteReceived = recvfrom(ReceivingSocket, ReceiveBuf, BufLength, 0,
                            (SOCKADDR *)&SenderAddr, &SenderAddrSize);
   if ( ByteReceived > 0 )
   {
        ///printf("Server: Total Bytes received: %d\n", ByteReceived);
        ///printf("Server: The data is \"%s\"\n", ReceiveBuf);
   }

   else if ( ByteReceived <= 0 )
        printf("Server: Connection closed with error code: %d\n", WSAGetLastError());
   else
       printf("Server: recvfrom() failed with error code: %d\n", WSAGetLastError());

   // Some info on the sender side
   getpeername(ReceivingSocket, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
   ///printf("Server: Sending IP used: %s\n", inet_ntoa(SenderAddr.sin_addr));
   ///printf("Server: Sending port used: %d\n", htons(SenderAddr.sin_port));



   // When your application is finished receiving datagrams close the socket.
   ///printf("Server: Finished receiving. Closing the listening socket...\n");

   if (closesocket(ReceivingSocket) != 0)
        printf("Server: closesocket() failed! Error code: %d\n", WSAGetLastError());
   ///else
        ///printf("Server: closesocket() is OK\n");

   // When your application is finished call WSACleanup.
   ///printf("Server: Cleaning up...\n");

   if(WSACleanup() != 0)
        printf("Server: WSACleanup() failed! Error code: %d\n", WSAGetLastError());
   ///else
     ///   printf("Server: WSACleanup() is OK\n");

   // Back to the system
   return 0;

}


int client_tcp() {

     WSADATA              wsaData;
     SOCKET               SendingSocket;

     // Server/receiver address
     SOCKADDR_IN          ServerAddr, ThisSenderInfo;

     // Server/receiver port to connect to
     unsigned int         Port = 2010;
     int  RetCode;

     // Be careful with the array bound, provide some checking mechanism...
     char sendbuf[1024] = "This is a test string from sender";
     int BytesSent, nlen;

     // Initialize Winsock version 2.2
     WSAStartup(MAKEWORD(2,2), &wsaData);
     ///printf("Client: Winsock DLL status is %s.\n", wsaData.szSystemStatus);

     // Create a new socket to make a client connection.
     // AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
     SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     if(SendingSocket == INVALID_SOCKET) {
          printf("Client: socket() failed! Error code: %d\n", WSAGetLastError());

          // Do the clean up
          WSACleanup();

          // Exit with error
          return -1;
     }
     ///else
          ///printf("Client: socket() is OK!\n");

     // Set up a SOCKADDR_IN structure that will be used to connect
     // to a listening server on port 2010. For demonstration
     // purposes, let's assume our server's IP address is 127.0.0.1 or localhost

     // IPv4
     ServerAddr.sin_family = AF_INET;

     // Port no.
     ServerAddr.sin_port = htons(Port);

     // The IP address
     ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


     // Make a connection to the server with socket SendingSocket.
     RetCode = connect(SendingSocket, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr));
     if(RetCode != 0) {
          printf("Client: connect() failed! Error code: %d\n", WSAGetLastError());

          // Close the socket
          closesocket(SendingSocket);

          // Do the clean up
          WSACleanup();

          // Exit with error
          return -1;
     } else {
          ///printf("Client: connect() is OK, got connected...\n");
          ///printf("Client: Ready for sending and/or receiving data...\n");
     }

     // At this point you can start sending or receiving data on
     // the socket SendingSocket.

     // Some info on the receiver side...
     getsockname(SendingSocket, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));
     ///printf("Client: Receiver IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
     ///printf("Client: Receiver port used: %d\n", htons(ServerAddr.sin_port));

     // Sends some data to server/receiver...
     //Sleep(1000);
    int cnt=0;
    int t=time(NULL);
     while (true){
         BytesSent = send(SendingSocket, sendbuf, sizeof(sendbuf), 0);
         ///cout<<BytesSent << " " << sendbuf<<endl;
         for (size_t i=0; i<strlen(sendbuf); i++) sendbuf[i]=0;
         BytesSent = recv(SendingSocket, sendbuf, sizeof(sendbuf),0);
         ///cout<<BytesSent << " " << sendbuf<<endl;
         for (size_t i=0; i<strlen(sendbuf); i++) sendbuf[i]=0;

         BytesSent = send(SendingSocket, "tigris1", sizeof("tigris1"), 0);
         ///cout<<BytesSent << " " << sendbuf<<endl;
         for (size_t i=0; i<strlen(sendbuf); i++) sendbuf[i]=0;
         BytesSent = recv(SendingSocket, sendbuf, sizeof(sendbuf),0);
         ///cout<<BytesSent << " " << sendbuf<<endl;
         for (size_t i=0; i<strlen(sendbuf); i++) sendbuf[i]=0;

         BytesSent = send(SendingSocket, "kutya2", sizeof("kutya2"), 0);
         ///cout<<BytesSent << " " << sendbuf<<endl;
         for (size_t i=0; i<strlen(sendbuf); i++) sendbuf[i]=0;
         BytesSent = recv(SendingSocket, sendbuf, sizeof(sendbuf),0);
         ///cout<<BytesSent << " " << sendbuf<<endl;
         for (size_t i=0; i<strlen(sendbuf); i++) sendbuf[i]=0;

         cnt++;
         if (t<time(NULL)){
            t=time(NULL);
            cout<<"FPS: "<<cnt<<endl;
            cnt=0;
         } else {
            ///cout<<t<<" "<<time(NULL)<<endl;
         }
     }

     if(BytesSent == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
     else {
          ///printf("Client: send() is OK - bytes sent: %ld\n", BytesSent);

          // Some info on this sender side...
          // Allocate the required resources
          memset(&ThisSenderInfo, 0, sizeof(ThisSenderInfo));
          nlen = sizeof(ThisSenderInfo);

          getsockname(SendingSocket, (SOCKADDR *)&ThisSenderInfo, &nlen);
          ///printf("Client: Sender IP(s) used: %s\n", inet_ntoa(ThisSenderInfo.sin_addr));
          ///printf("Client: Sender port used: %d\n", htons(ThisSenderInfo.sin_port));
          ///printf("Client: Those bytes represent: \"%s\"\n", sendbuf);
     }

     if( shutdown(SendingSocket, SD_SEND) != 0)
          printf("Client: Well, there is something wrong with the shutdown(). The error code: %d\n", WSAGetLastError());
     ///else
          ///printf("Client: shutdown() looks OK...\n");

     // When you are finished sending and receiving data on socket SendingSocket,
     // you should close the socket using the closesocket API. We will
     // describe socket closure later in the chapter.
     if(closesocket(SendingSocket) != 0)
          printf("Client: Cannot close \"SendingSocket\" socket. Error code: %d\n", WSAGetLastError());
     ///else
          ///printf("Client: Closing \"SendingSocket\" socket...\n");

     // When your application is finished handling the connection, call WSACleanup.
     if(WSACleanup() != 0)
          printf("Client: WSACleanup() failed!...\n");
     ///else
          ///printf("Client: WSACleanup() is OK...\n");

     return 0;
}


SOCKET client_tcp_create_socket() {
     WSADATA              wsaData;
     SOCKET               SendingSocket;

     WSAStartup(MAKEWORD(2,2), &wsaData);
     ///printf("Client: Winsock DLL status is %s.\n", wsaData.szSystemStatus);

     // Create a new socket to make a client connection.
     // AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
     SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     if(SendingSocket == INVALID_SOCKET) {
          printf("Client: socket() failed! Error code: %d\n", WSAGetLastError());

          // Do the clean up
          WSACleanup();

          // Exit with error
          return SOCKET_ERROR;
     }
     ///else
          ///printf("Client: socket() is OK!\n");
    return SendingSocket;
}

int client_tcp_connect(SOCKET socket) {
     SOCKADDR_IN ServerAddr;
     // Set up a SOCKADDR_IN structure that will be used to connect
     // to a listening server on port 2010. For demonstration
     // purposes, let's assume our server's IP address is 127.0.0.1 or localhost

     // IPv4
     ServerAddr.sin_family = AF_INET;

     // Port no.
     ServerAddr.sin_port = htons(PORT);

     // The IP address
    ServerAddr.sin_addr.s_addr =inet_addr("127.0.0.1");
     ///ServerAddr.sin_addr.s_addr = *(u_long *)gethostbyname("medvekilatas.ddns.net")->h_addr_list[0];

    int RetCode;
    RetCode = connect(socket, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr));
     if(RetCode != 0) {
          printf("Client: connect() failed! Error code: %d\n", WSAGetLastError());

          // Close the socket
          closesocket(socket);

          // Do the clean up
          WSACleanup();

          // Exit with error
          return -1;
     } else {
          ///printf("Client: connect() is OK, got connected...\n");
          ///printf("Client: Ready for sending and/or receiving data...\n");
     }

     // At this point you can start sending or receiving data on
     // the socket SendingSocket.

     // Some info on the receiver side...
     getsockname(socket, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));
     printf("Client: Receiver IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
     printf("Client: Receiver port used: %d\n", htons(ServerAddr.sin_port));

     return 0;
}

void client_tcp_send_string(SOCKET socket, string sendbuff) {
    int BytesSent;
    BytesSent = send(socket, sendbuff.c_str(), sendbuff.size(), 0);

    if(BytesSent == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
}

void client_tcp_send_char(SOCKET socket, char* sendbuff) {
    int BytesSent;
    BytesSent = send(socket, sendbuff, strlen(sendbuff)+2, 0);
    ///cout<<sendbuff<<" "<<strlen(sendbuff)<<" "<<BytesSent<<endl;

    if(BytesSent == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
}

int client_tcp_recv_char(SOCKET socket, char* recvbuff) {
    rst_recvbuff(recvbuff);
    int BytesReceived;
    BytesReceived = recv(socket, recvbuff, 1024, 0);
    ///cout<<recvbuf<<endl;
    if(BytesReceived == SOCKET_ERROR)
          printf("Client: send() error %d.\n", WSAGetLastError());
    return BytesReceived;
}

int client_tcp_close_socket(SOCKET socket) {
    if( shutdown(socket, SD_SEND) != 0)
          printf("Client: Well, there is something wrong with the shutdown(). The error code: %d\n", WSAGetLastError());
     ///else
          ///printf("Client: shutdown() looks OK...\n");

     // When you are finished sending and receiving data on socket SendingSocket,
     // you should close the socket using the closesocket API. We will
     // describe socket closure later in the chapter.
     if(closesocket(socket) != 0)
          printf("Client: Cannot close \"SendingSocket\" socket. Error code: %d\n", WSAGetLastError());
     ///else
          ///printf("Client: Closing \"SendingSocket\" socket...\n");

     // When your application is finished handling the connection, call WSACleanup.
     if(WSACleanup() != 0)
          printf("Client: WSACleanup() failed!...\n");
     ///else
          ///printf("Client: WSACleanup() is OK...\n");

     return 0;
}

void clear_recvbuf(char* recvbuf) {
    for (int i=0; i<1024; i++) recvbuf[i]=0;
}
