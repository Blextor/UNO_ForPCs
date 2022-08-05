#include "Game.h"

using namespace std;



int main(){

    ///########################################
    /// Kapcsolódás a szerverhez
    ///########################################

    char connect_msg_char[1024];                    // ideglenes változó, a szervernek a kapcsolódási módot küldi el
    connect_msg(connect_msg_char);                  // létrehozza magát az üzenetet


    ///########################################
    /// Socket létrehozása
    ///########################################

    SOCKET socket = client_tcp_create_socket();  // socket-tel kommunikál a program
    client_tcp_connect(socket);

    char recvbuff[1024];                         // ebben fogadja az adatokat a szervertõl
    char sendbuff[1024];
    /// EDDIG TART
    cout<<"SOCKET CREATED!"<<endl;
    ///########################################

    client_tcp_send_char(socket,connect_msg_char);  // elküldi a szervernek
    cout<<"Elküldve: "<<connect_msg_char<<endl;
    client_tcp_recv_char(socket,recvbuff);          // fogadja a válaszát
    cout<<"Fogadva: "<<recvbuff<<endl;

    if (!server_accept_ok(recvbuff)){               // ha a szerver elutasította a kérést
        printf("%s",recvbuff);                      // kiírja, hogy miért utasította el a szerver
        Sleep(10000);
        return (-1);                                // és akkor vége a programnak
    }
    /// EDDIG TART                                   // ezt meg kell tenni mindig, különben a régi üzenet darabjai benne maradhatnak
    cout<<"CONNECTED!"<<endl;
    ///########################################


    string command;                                 // ebben fogom eltárolni, a szerver üzeneteinek lényegét (a parancsot)
    bool on=true;                                   // ebben tárolom azt, hogy vége van-e az összes játéknak
    while (on) {                                    // addig megy, míg vége nincs az összesnek


        ///########################################
        /// Játék felépítése
        ///########################################

        client_tcp_send_string(socket,"READY");       // eddig a pontig kell felkészíteni egy (újabb) játékra a programot

        client_tcp_recv_char(socket,recvbuff);      // itt kapja meg a játékról az adatokat  S: DATA...
        cout<<recvbuff<<endl;                       // a játékosok számát, a saját sorszámát, és a lapjait

        ///#################################
        /// INNENTÕL MÓDOSÍTHATÓ (az adatok elmentése)  /// DATA <játekosok szama> <sorszám a játékosnak> <kezdő lap> 7x<lapok>
        Game game(recvbuff);                        // elmenti a kaptott adatokat, ez mindenki kénye kedvére módosíthatja (csak okosan)
        ///  IDÁIG MÓDOSÍTHATÓ
        ///#################################

        client_tcp_send_string(socket,"OK");          // küld egy OK-t hogy ezzel is megvolna, és vár, hogy õ jöhessen
        /// EDDIG TART
        cout<<"READY FOR NEW GAME!"<<endl;
        ///########################################




        command="";                                 // reset-eli a command-et
        while (command!="END"){                     // és amíg a szerver nem küldi neki, hogy az aktuális játéknak vége, addig pörög a ciklus
            ///########################################
            /// Játék
            ///########################################

            ///int Bytes=
            client_tcp_recv_char(socket,recvbuff);  // megkapja a játékos, hogy mi következik
                                                    // az alábbi három történhet: õ jön, nyert, vége a játéknak
            //get_command(command, recvbuff);         // kiszedi belõle a parancsot
            stringstream s; s<<recvbuff; s>>command;
                                                    // amely alapján dönt, hogy miként fut tovább a program;
            ///cout<<Bytes<<" "<<command<<" "<<(command=="YT!")<<" "<<recvbuff<<endl;
            if (DEBUG)  cout<<"Server: "<<recvbuff<<endl;
            if (command=="YT!") { // te köröd, lehet hogy simán jöhetsz, kimaradsz, vagy plusz-os lap aktív
                s>>command;                                                              /// EZ HIÁNYZIK "CSAK" :D
                if (command=="PLUS"){ /// MI TÖRTÉNJEN, HA PLUSZ-OS LAP AKTÍV
                    if (!game.put_Pcard(recvbuff, sendbuff)) {
                        client_tcp_send_string(socket,"DRAW");
if (DEBUG)              cout<<"Kliens: "<<"DRAW"<<endl;
                        client_tcp_recv_char(socket,recvbuff); /// S: CARD(S) ...
if (DEBUG)              cout<<"Server: "<<recvbuff<<endl;
                        //get_command(command,recvbuff);
                        game.get_card(recvbuff);            /// esetleg hibaellenőrzés
                        client_tcp_send_string(socket,"OK");
if (DEBUG)              cout<<"Kliens: "<<"OK"<<endl;

                    }
                    else {
                        client_tcp_send_char(socket,sendbuff); /// C: PUT ...
if (DEBUG)              cout<<"Kliens: "<<sendbuff<<endl;
                    }
                }
                else if (command=="OFF"){   /// HA KIMARADSZ
                    client_tcp_send_string(socket,"OK"); // elküldi a szervernek, hogy tudomásul vette, hogy kimarad
if (DEBUG)          cout<<"Kliens: "<<"OK"<<endl;
                }
                else if (command=="NONE"){  /// HA NINCS SEMMI KÜLÖNÖS
                    if (!game.put_card(recvbuff, sendbuff)) {
                        //Sleep(10000);
                        client_tcp_send_string(socket,"DRAW");
if (DEBUG)              cout<<"Kliens: "<<"DRAW"<<endl;
                        client_tcp_recv_char(socket,recvbuff); /// S: CARD(S) ...
if (DEBUG)              cout<<"Server: "<<recvbuff<<endl;
                        //get_command(command,recvbuff);
                        game.get_card(recvbuff);            /// esetleg hibaellenőrzés
                        client_tcp_send_string(socket,"OK");
if (DEBUG)              cout<<"Kliens: "<<"OK"<<endl;
                    }
                    else{
                        client_tcp_send_char(socket,sendbuff); /// C: PUT ...
if (DEBUG)              cout<<"Kliens: "<<sendbuff<<endl;
                    }
                }
                else {
                    client_tcp_send_char(socket,"ERROR");
if (DEBUG)          cout<<"Kliens: "<<"ERROR"<<endl;
                }
            }
            else if(command=="WIN"){ // WIN / WON, mindegy, lényeg, hogy nyert. Ilyenkor a következõ az END parancs lesz.
if (DEBUG)      cout<<"Helyezett: "<<recvbuff[4]<<endl;
                client_tcp_send_string(socket,"OK");
            }
            else if(command=="END"){ // Vége a játéknak. Vagy teljesen, vagy van következõ játék.
                if (recvbuff[4]=='0')
                    on=false;
                //rst_recvbuff(command);                      // reset-eli a command-et
            }
            else {
if (DEBUG)      cout<<command<<endl<<(command=="YT!")<<endl;
                on=false;
                break;
            }

        }

    }           // kilép, ha az összes játék lefutott


    ///########################################
    /// Vége, eredmények
    ///########################################

    client_tcp_send_string(socket,"BYE");     // elküldi a szervernek, hogy tudja, hogy vége van a játékoknak
    rst_recvbuff(recvbuff);                 // reseteli a recvbuff-ot, tiszta lapról indul
    client_tcp_recv_char(socket,recvbuff);  // megkapja a végeredményeket
    cout<<recvbuff<<endl;                   // amiket ki is ír (ez kissé nyersen)

    /// EDDIG TART
    ///########################################



    ///Sleep(5000);
    ///
     int i; cin>>i; cout<<i;
}
