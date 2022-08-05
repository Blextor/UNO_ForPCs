#include "Game.h"

using namespace std;
using namespace std::chrono;

char num_to_char(int i) {return 48+i;}

void prepare(vector<Client> &clients, vector<Game> &games) {
    clients.resize(client_cnt);
    ifstream fpi;
    fpi.open("IDs.txt");
    for (int i=0; i<client_cnt; i++){
        clients[i].socket=server_tcp_create_listening_socket();
        u_short port=2020+i;
        server_tcp_bind(clients[i].socket,port);
        server_tcp_start_listening(clients[i].socket);
        clients[i].listening=true;
        fpi>>clients[i].id;
        cout<<"Listening: "<<i<<endl;///" "<<clients[i].id<<" "<<port<<endl;
    }

    for (int i=2; i<=8; i++){
        for (int j=0; j<=i-2; j++){
            games.push_back(Game(i*10+j));
        }
    }
    for (int i=1; i<=7; i++) {
        games.push_back(Game(10+11*i));
    }
    games[0].set_kliensek(clients);
}

int roomNum_to_gameNum(int num){
    int c=0;
    for (int i=2; i<=8; i++){
        for (int j=0; j<=i-2; j++){
            if (i*10+j==num)
                return c;
            c++;
        }
    }
    for (int i=1; i<=7; i++){
        if (10+11*i==num)
            return c;
        c++;
    }
    return -1;
}

void rendezett_pakli (vector<Card> inp) {
    char lap[3]; lap[2]=0;
    Card temp;
    for (int i=0; i<52; i++){   /// Színes számos lapok
        if (i/13==0)
            lap[0]='R';
        else if (i/13==1)
            lap[0]='G';
        else if (i/13==2)
            lap[0]='B';
        else if (i/13==3)
            lap[0]='Y';

        if (i%13<10)
            lap[1]=num_to_char(i%13);
        else if (i%13==10)
            lap[1]='P';
        else if (i%13==11)
            lap[1]='S';
            else if (i%13==12)
            lap[1]='L';
        temp.fullset(lap);
        //temp.kiir();
        inp.push_back(temp);
        if (i%13!=0)
            inp.push_back(temp);
        //temp.kiir();
        cout<<inp.size()<<endl;
    }

    lap[0]='J';
    for (int i=0; i<4; i++) {

        lap[1]='P';
        temp.fullset(lap);
        inp.push_back(temp);
        inp.push_back(temp);
        //temp.kiir();

        lap[1]='C';
        temp.fullset(lap);
        inp.push_back(temp);
        inp.push_back(temp);
        //temp.kiir();
    }

}

void keveres (list<Card> &pakli) {
    char lap[3]; lap[2]=0;
    Card temp;

    vector<Card> inp;
    for (int i=0; i<52; i++){   /// Színes számos lapok
        if (i/13==0)
            lap[0]='R';
        else if (i/13==1)
            lap[0]='G';
        else if (i/13==2)
            lap[0]='B';
        else if (i/13==3)
            lap[0]='Y';

        if (i%13<10)
            lap[1]=num_to_char(i%13);
        else if (i%13==10)
            lap[1]='P';
        else if (i%13==11)
            lap[1]='S';
            else if (i%13==12)
            lap[1]='L';
        temp.fullset(lap);
#ifdef DEBUG_CMD
        //temp.kiir();
#endif // DEBUG_CMD
        inp.push_back(temp);
        if (i%13!=0)
            inp.push_back(temp);
#ifdef DEBUG_CMD
        //temp.kiir();
        cout<<inp.size()<<endl;
#endif // DEBUG_CMD
    }

    lap[0]='J';
    for (int i=0; i<4; i++) {

        lap[1]='P';
        temp.fullset(lap);
        inp.push_back(temp);
#ifdef DEBUG_CMD
        //temp.kiir();
#endif // DEBUG_CMD

        lap[1]='C';
        temp.fullset(lap);
        inp.push_back(temp);
#ifdef DEBUG_CMD
        //temp.kiir();
#endif // DEBUG_CMD
    }

    for (int i=0; i<108; i++){
#ifdef DEBUG_CMD
        //cout<<inp.size();
#endif // DEBUG_CMD
        int no=rand()%inp.size();
#ifdef DEBUG_CMD
        cout<<no<<endl;
#endif // DEBUG_CMD
        pakli.push_back(inp[no]);
#ifdef DEBUG_CMD
        inp[no].kiir();
#endif // DEBUG_CMD
        inp.erase(inp.begin()+no);

    }
}

bool is_starter_card(std::string str){
    if (str[1]!='C' && str[1]!='P' && str[1]!='L' && str[1]!='S')
        return true;
    return false;
}

std::vector<Client>* Game::kliensek;

void Game::reset(std::vector<Client> &clients) {
    last_msg_time=0;
    //cout<<clients.size()<<" "<<kliensek[0].size()<<endl;
    //if (clients.size()!=kliensek[0].size()){
        ///cout<<"Game reset "<<kliensek[0].size()<<" almafa"<<endl;
        for (int i=0; i<player_cnt; i++){
            ///cout<<players[i].client.id<<endl;
            for (size_t j=0; j<kliensek[0].size(); j++){
                ///cout<<kliensek[0][j].id<<endl;
                if (players[i].client.id==kliensek[0][j].id){
                    ///cout<<i<<" "<<j<<endl;
                    kliensek[0][j].real_reset();
                    ///cout<<i<<" "<<j<<endl;
                }
            }
        }
    //}
    //for (int i=0; i<player_cnt; i++)
      //  players[i].client.reset();
    players.resize(0);
    players.resize(player_cnt);

    points.resize(0);
    points.resize(player_cnt-robot_cnt);
    points_name.resize(0);
    points_name.resize(player_cnt-robot_cnt);

    pakli.resize(0);
    keveres(pakli);

    for (int i=0; i<player_cnt-robot_cnt; i++) points.push_back(0);
    for (int i=0; i<player_cnt-robot_cnt; i++) points_name.push_back("");

    dobopakli.resize(0);
    osztas();

    way=true;
    data_sent_cnt=0;
    actual_player=0; active_players=0;
    won_players=0;
    draw_cards_cnt=0;
    games_count=0;
    left_out_cnt=0;
    state=wait;
    is_lastCardActive=false; ended_game=false;
    last_card.fullset(dobopakli.front().get_str());
    intrested_color=last_card.get_color(); intrested_type=last_card.get_type();

    vector<int> ord_num;
    for (int i=0; i<player_cnt; i++) {
        ord_num.push_back(i);
        players[i].setPlayer();
    }
    for (int i=0; i<robot_cnt; i++){
        int temp = rand()%(player_cnt-i);
        players[ord_num[temp]].setRobot();
        ord_num.erase(ord_num.begin()+temp);
    }

    time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
    time_ms2 = time_ms1;
}

void Game::shuffel_reset() {
    vector<Client> clients_temp;
    for (int i=0; i<player_cnt; i++){
        if (!players[i].robot)
            clients_temp.push_back(players[i].client);
    }

    players.resize(0);
    players.resize(player_cnt);

    int cnt=0, robo_cnt=robot_cnt, ply_cnt=0;
    for (int i=0; i<player_cnt; i++){
        if ((rand()%2==0 && robo_cnt>0) || ply_cnt==player_cnt-robot_cnt){
            robo_cnt--;
            players[i].setRobot();
        } else {
            cnt=rand()%clients_temp.size();
            players[i].client=clients_temp[cnt];
            clients_temp.erase(clients_temp.begin()+cnt);
            ply_cnt++;
        }
    }

    pakli.resize(0);
    keveres(pakli);

    dobopakli.resize(0);
    osztas();

    way=true;
    data_sent_cnt=0;
    actual_player=0; active_players=player_cnt-robot_cnt;
    won_players=0;
    draw_cards_cnt=0;
    left_out_cnt=0;
    state=wait;
    is_lastCardActive=false; ended_game=false;
    last_card.fullset(dobopakli.front().get_str());
    intrested_color=last_card.get_color(); intrested_type=last_card.get_type();

    time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
    time_ms2 = time_ms1;
}

void Game::osztas (){
    for (int i=0; i<7; i++) {
        for (int j=0; j<player_cnt; j++){
            Card temp = pakli.front();
            pakli.pop_front();
            players[j].addCard(temp);
        }
    }
    while (true) {
        Card temp=pakli.front();
        pakli.pop_front();
        if (is_starter_card(temp.get_str())){
            dobopakli.push_back(temp);
            break;
        } else
            pakli.push_back(temp);
    }
#ifdef DEBUG_CMD
    for (int i=0; i<player_cnt; i++)
        players[i].cards_write();
#endif // DEBUG_CMD
}

void Game::set_kliensek(std::vector<Client> &clients){
    kliensek=&clients;
}

Game::Game(int room_num) {
    player_cnt=room_num/10;
    robot_cnt=room_num%10;
    last_msg_time=0;

    players.resize(0);
    players.resize(player_cnt);

    points.resize(0);
    //points.resize(player_cnt-robot_cnt);
    points_name.resize(0);
    //points_name.resize(player_cnt-robot_cnt);

    pakli.resize(0);
    keveres(pakli);

    for (int i=0; i<player_cnt-robot_cnt; i++) points.push_back(0);
    for (int i=0; i<player_cnt-robot_cnt; i++) points_name.push_back("");

    dobopakli.resize(0);
    osztas();

    way=true;
    data_sent_cnt=0;
    actual_player=0; active_players=0;
    games_count=0; games_max=GAME_COUNT;
    won_players=0;
    draw_cards_cnt=0;
    left_out_cnt=0;
    state=wait;
    is_lastCardActive=false; ended_game=false;
    last_card.fullset(dobopakli.front().get_str());
    intrested_color=last_card.get_color(); intrested_type=last_card.get_type();

    vector<int> ord_num;
    for (int i=0; i<player_cnt; i++) {
        ord_num.push_back(i);
        players[i].setPlayer();
    }
    for (int i=0; i<robot_cnt; i++){
        int temp = rand()%(player_cnt-i);
        players[ord_num[temp]].setRobot();
        ord_num.erase(ord_num.begin()+temp);
    }
    time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
    time_ms2 = time_ms1;
    time_sec1=std::time(NULL);
    time_sec2=time_sec1;
}

bool is_char_ready(char* str){
    if (str[0]=='R' && str[1]=='E' && str[2]=='A' && str[3]=='D' && str[4]=='Y')
        return true;
    return false;
}

void Game::Server_getReadys(std::vector<Client> &clients) {
    for (int i=0; i<player_cnt; i++){
        char recvbuff[1024];
        if (!players[i].robot){
            server_tcp_recv_char(players[i].client.socket, recvbuff);
            if (!is_char_ready(recvbuff)){
                cout<<"Server_getReadys() hiba, jatekos uzenete nem stimmel! Jatekos: "<<players[i].client.id<<", uzenete: "<<recvbuff<<endl;
                reset(clients);
                return;
            }
        }
    }
    state=ready;
}

void card_cpy_str_to_char(std::string str, char* card) {
    card[0]=str[0]; card[1]=str[1];
}

void Game::Server_Data_send() {
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Data_send"<<endl;
    }


    for (int i=0; i<player_cnt; i++){
        char sendbuff[1024];
        for (int j=0; j<1024; j++) sendbuff[j]=0;

        char card[3];
        if (!players[i].robot){
            strcpy(sendbuff,"DATA ");
            sendbuff[5]=num_to_char(player_cnt);
            strcat(sendbuff," ");
            sendbuff[7]=num_to_char(i);
            strcat(sendbuff," ");
            card_cpy_str_to_char(last_card.get_str(), card);
            strcat(sendbuff,card);
            for (int j=0; j<7; j++){
                strcat(sendbuff," ");
                card_cpy_str_to_char(players[i].cards[j].get_str(), card);
                strcat(sendbuff, card);
                ///cout<<sendbuff<<endl;
            }
            ///cout<<sendbuff<<endl;
            server_tcp_send_char(players[i].client.socket,sendbuff);
        }
    }
    state=data_sent;
}

void Game::Server_Data_ok(std::vector<Client> &clients) {
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Data_ok"<<endl;
    }

    for (int i=0; i<player_cnt; i++) {
        char ok[1024];
        if (!players[i].robot){
            server_tcp_recv_char(players[i].client.socket, ok);
            if (!(ok[0]='O' && ok[1]=='K')){
                cout<<"Server_Data_ok() hiba, az uzenet nem stimmel! Jatekos: "<<players[i].client.id<<", uzenet: "<<ok<<endl;
                reset(clients);
                return;
            }

        }
    }
    if (games_count==0)
        cout<<"Server_Data_ok() sikerult!"<<endl;
    state=data_ok;
}

void Game::Server_End(std::vector<Client> &clients) {
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_End"<<endl;
    }


    games_count++;
    if (DEBUG_CMD_game_count && 0==games_count%DEBUG_CMD_game_cnt) cout<<"Game: "<<player_cnt*10+robot_cnt<<", games_count: "<<games_count<<endl;
    char msg[6]; strcpy(msg,"END ");
    if (games_count>=games_max)
        msg[4]=48+0;
    else
        msg[4]=48+1;
    msg[3]=' '; msg[5]=0;
    for (int i=0; i<player_cnt; i++){
        if (!players[i].robot){
            server_tcp_send_char(players[i].client.socket, msg);
            if (!players[i].won){
                for (size_t j=0; j<points_name.size(); j++){
                    if (points_name[j]==players[i].client.id){
                        points[j]+=player_cnt;            /// Ha nem marad a kezében lap, akkor ne kapjon +1 pontot?
                        //if (players[i].zeroCards())
                            //points[j]--;
                    }
                }
            }

        }
    }
    if (games_count>=games_max){
        char recvbuff[1024];
        for (int i=0; i<1024; i++) recvbuff[i]=0;
        for (int i=0; i<player_cnt; i++){
            if (!players[i].robot){
                server_tcp_recv_char(players[i].client.socket,recvbuff);
                if (!(recvbuff[0]=='B' && recvbuff[1]=='Y' && recvbuff[2]=='E')){
                    cout<<"Server_End() hiba, az uzenet nem stimmel! Jatekos: "<<players[i].client.id<<", uzenet: "<<recvbuff<<endl;
                    players[i].client.reset();
                }
            }
        }
        state=stop;
    }
    else {
        shuffel_reset();
        state=wait;
    }
}

void Game::Server_Cmd(char* sendbuff, int sft) {   /// HISTORY DONE (CSAK A KIÍRÁSÉRT FELEL)
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Cmd"<<endl;
    }

    sendbuff[9-sft]=last_card.get_str()[0]; sendbuff[10-sft]=last_card.get_str()[1]; sendbuff[11-sft]=' ';
    sendbuff[12-sft]=intrested_color; sendbuff[13-sft]=' ';
    sendbuff[14-sft]=intrested_type; sendbuff[15-sft]=' ';
    if (way)
        sendbuff[16-sft]='0';
    else
        sendbuff[16-sft]='1';
    sendbuff[17-sft]=' '; sendbuff[18-sft]=0;
    if (sft!=1){
        strcat(sendbuff,"HISTORY ");
        for (size_t i=0; i<players[actual_player].history.size(); i++){
            strcat(sendbuff, players[actual_player].history[i].c_str());
        }
        players[actual_player].history.resize(0);
    }
}

void Game::Server_None(std::vector<Client> &clients) {   /// HISTORY DONE (TRANZ)
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_None"<<endl;
    }

    char sendbuff[1024];
    strcpy(sendbuff,"YT! NONE ");
    Server_Cmd(sendbuff,0);
    server_tcp_send_char(players[actual_player].client.socket, sendbuff);
    char recvbuff[1024];
    server_tcp_recv_char(players[actual_player].client.socket, recvbuff);
    stringstream s;
    s<<recvbuff;
    string cmd;
    s>>cmd;
    if (cmd=="DRAW") { Server_Cards(clients);}
    else if (cmd=="PUT") { Server_Put(recvbuff, clients);}
    else {
        cout<<"Server_None() hiba, nem stimmel az uzenet! Jatekos: "<<players[actual_player].client.id<<", uzenet: "<<recvbuff<<endl;
        reset(clients);
    }
}

void Game::Server_None_robot() {   /// ROBOT DONE (TRANZ)
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_None_robot"<<endl;
    }

    Server_Put_robot();
}

void Game::Server_Off(std::vector<Client> &clients) {
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Off"<<endl;
    }

    if (players[actual_player].robot) {

    }
    else {
        char sendbuff[1024];
        strcpy(sendbuff,"YT! OFF ");
        Server_Cmd(sendbuff,1);
        server_tcp_send_char(players[actual_player].client.socket, sendbuff);
        char recvbuff[1024];
        server_tcp_recv_char(players[actual_player].client.socket, recvbuff);
        if (recvbuff[0]=='O' && recvbuff[1]=='K'){
            left_out_cnt--;
            if (left_out_cnt==0)
                is_lastCardActive=false;
            Next_Player();
        }
        else {
            cout<<"Server_Off() hiba, nom Ok-zott uzenet. Jatekos uzenete: "<<recvbuff<<endl;
            reset(clients);
        }
    }
}

void Game::Server_Off_robot() {     /// ROBOT DONE
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Off_robot"<<endl;
    }

    left_out_cnt--;
    if (left_out_cnt==0)
        is_lastCardActive=false;
    Next_Player();
}

void Game::Server_Plus(std::vector<Client> &clients) {   /// HISTORY DONE (TRANZ)
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Plus"<<endl;
    }
    if (players[actual_player].robot) {

    }
    else {
        char sendbuff[1024];
        strcpy(sendbuff,"YT! PLUS ");
        Server_Cmd(sendbuff,0);
        server_tcp_send_char(players[actual_player].client.socket, sendbuff);
        char recvbuff[1024];
        server_tcp_recv_char(players[actual_player].client.socket, recvbuff);
        stringstream s;
        s<<recvbuff;
        string cmd;
        s>>cmd;
        if (cmd=="DRAW") { Server_Cards(clients); return;}
        else if (cmd=="PUT") { Server_PPut(recvbuff, clients); return;}
        else {
            cout<<"Server_None() hiba, nem stimmel az uzenet! Jatekos: "<<players[actual_player].client.id<<", uzenet: "<<recvbuff<<endl;
            reset(clients);
        }
    }
}

void Game::Server_Plus_robot() {   /// ROBOT DONE   HISTORY DONE (TRANZ)
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Plus_robot"<<endl;
    }
    if (!Server_PPut_robot()){
        Server_Cards_robot();
    }
    Next_Player();
}

void Game::shuffle_dobopakli(int remain, std::vector<Client> &clients) {   /// HISTORY DONE
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"shuffle_dobopakli"<<endl;
    }

    int cnt=dobopakli.size();
    if (cnt<=remain) {
        if (players[actual_player].robot){
            cout<<"Elfogyott a huzopakli a robotnal! Game: "<<player_cnt*10+robot_cnt<<endl;
            for (int i=0; i<player_cnt; i++){
                if (!players[i].robot){
                    for (size_t j=0; j<kliensek[0].size(); j++){
                        if (kliensek[0][j].id==players[i].client.id){
                            kliensek[0][j].real_reset();
                        }
                    }
                }
            }
            reset(kliensek[0]);
        } else {
            cout<<"shuffle_dobopakli() hiba, nincs eleg lap! Jatekos: "<<players[actual_player].client.id<<", lapok szama: "<<players[actual_player].cards.size()<<endl;
            reset(clients);
        }
        return;
    } else {
        vector<Card> temp;
        for (int i=0; i<cnt-1; i++) {
            temp.push_back(dobopakli.front());
            dobopakli.pop_front();
        }
        for (int i=0; i<cnt-1; i++){
            int ran=rand()%(cnt-1-i);
            pakli.push_back(temp[ran]);
            temp.erase(temp.begin()+ran);
        }
    }
    for (int i=0; i<player_cnt; i++){
        if (!players[i].robot){
            players[i].history.push_back("SHUFFLE ");
        }
    }
}

void Game::Server_Cards(std::vector<Client> &clients) {   /// HISTORY DONE
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Cards"<<endl;
    }

    char cardsbuff[1024];
    strcpy(cardsbuff,"CARD(S)");
    if (draw_cards_cnt==0)
        draw_cards_cnt=1;

    stringstream s;
    s<<"DRAW "<<actual_player<<" "<<draw_cards_cnt<<" ";
    string history_str=s.str();
    for (int i=0; i<player_cnt; i++){
        if (!players[i].robot && i!=actual_player)
            players[i].history.push_back(history_str);
    }

    for (int i=0; i<draw_cards_cnt; i++){
        if (pakli.size()==0)
            shuffle_dobopakli(draw_cards_cnt-i, clients);
        cardsbuff[7+i*3]=' ';
        cardsbuff[8+i*3]=pakli.front().get_color();
        cardsbuff[9+i*3]=pakli.front().get_type();
        players[actual_player].cards.push_back(pakli.front());
        pakli.pop_front();
        cardsbuff[10+i*3]=0;
    }
    draw_cards_cnt=0;
    is_lastCardActive=false;
    server_tcp_send_char(players[actual_player].client.socket, cardsbuff);
    server_tcp_recv_char(players[actual_player].client.socket, cardsbuff);
    Next_Player();
    if (cardsbuff[0]=='O' && cardsbuff[1]=='K')
        return;
    cout<<"Server_Cards() hiba, nom Ok-zott uzenet. Jatekos uzenete: "<<cardsbuff<<endl;
    reset(clients);
}

void Game::Server_Cards_robot() {  /// ROBOT DONE   HISTORY DONE
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Cards_robot"<<endl;
    }

    if (draw_cards_cnt==0)
        draw_cards_cnt=1;

    stringstream s;
    s<<"DRAW "<<actual_player<<" "<<draw_cards_cnt<<" ";
    string history_str=s.str();
    for (int i=0; i<player_cnt; i++){
        if (!players[i].robot && i!=actual_player)
            players[i].history.push_back(history_str);
    }
    for (int i=0; i<draw_cards_cnt; i++){
        if (pakli.size()==0)
            shuffle_dobopakli(draw_cards_cnt-i, kliensek[0]);
        players[actual_player].cards.push_back(pakli.front());
        pakli.pop_front();
    }
    draw_cards_cnt=0;
    is_lastCardActive=false;
}

bool Game::is_capable_card(std::string next){
    if (DEBUG_CMD_commands) cout<<"is_capable_card"<<endl;
    if ((next[0]==intrested_color || next[1]==intrested_type) && next[0]!='J')
        return true;
    return false;
}

void Game::Next_Player() {
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Next_Player"<<endl;
    }

    for (int i=0; i<player_cnt-1; i++){
        if (way)
            actual_player++;
        else
            actual_player--;
        if (actual_player<0)
            actual_player+=player_cnt;
        if (actual_player>=player_cnt)
            actual_player-=player_cnt;
        if (!players[actual_player].won)
            break;
    }
}

void Game::Server_Put(char* recvbuff, std::vector<Client> &clients) { /// HISTORY DONE, Több lap egyszerre DONE (ellenőrzés szükséges)
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Put"<<endl;
    }

    vector<Card> temp;
    string cmd;
    stringstream s; s<<recvbuff;
    s>>cmd;
    Card card;
    s>>cmd;

    if (cmd[0]=='J' && (cmd[1]=='P' || cmd[1]=='C')){
        for (size_t i=0; i<players[actual_player].cards.size(); i++){
            if (players[actual_player].cards[i].get_str()==cmd){
                players[actual_player].cards.erase(players[actual_player].cards.begin()+i);
                card.fullset(cmd);
                dobopakli.push_back(card);
                last_card=card;

                if (cmd[1]=='P'){
                    draw_cards_cnt=4;
                    is_lastCardActive=true;
                }
                s>>cmd;
                if (cmd[0]=='R' || cmd[0]=='G' || cmd[0]=='B' || cmd[0]=='Y'){
                    intrested_color=cmd[0];
                    intrested_type='N';
                } else {
                    cout<<"Server_Put() hiba, nem kert jol szint a jatekos! Jatekos: "<<players[actual_player].client.id<<", uzenet: "<<recvbuff<<endl;
                    reset(clients);
                }

                stringstream s;
                s<<"PUT "<<actual_player<<" 1 "<<card.get_str()<<" "<<intrested_color<<" ";    /// HISTORY
                string history_str=s.str();
                for (int k=0; k<player_cnt; k++){
                    if (!players[k].robot && k!=actual_player)
                        players[k].history.push_back(history_str);
                }

                Next_Player();
                return;
            }
            if (i==players[actual_player].cards.size()-1){
                cout<<"Server_Put() hiba, nincs ilyen lapja a jatekosnak! Jatekos: "<<players[actual_player].client.id<<", lap: "<<cmd<<endl;
                reset(clients);
                return ;
            }
        }
    }
    while (true){
        if (cmd.size()==2){
            card.fullset(cmd);
            temp.push_back(card);
            if (s.eof())
                break;
        } else {
            cout<<"Server_Put() hiba, hibas uzenet, nincs ilyen lap! Jatekos: "<<players[actual_player].client.id<<", lap: "<<recvbuff<<endl;
            reset(clients);
            return;
        }
        s>>cmd;
    }
    if (temp.size()<=0){
        cout<<"Server_Put() hiba, hibas uzenet, nem kuldott lapot! Jatekos: "<<players[actual_player].client.id<<", lap: "<<recvbuff<<endl;
        reset(clients);
        return;
    }
    else if (temp.size()==1){
        if (is_capable_card(temp[0].get_str())){
            for (size_t i=0; i<players[actual_player].cards.size(); i++){
                if (players[actual_player].cards[i].get_str()==temp[0].get_str()){
                    players[actual_player].cards.erase(players[actual_player].cards.begin()+i);
                    dobopakli.push_back(temp[0]);
                    last_card.fullset(temp[0].get_str());
                    intrested_color=temp[0].get_color();
                    intrested_type=temp[0].get_type();
                    if (intrested_type=='P'){
                        draw_cards_cnt=2;
                        is_lastCardActive=true;
                    }
                    else if (intrested_type=='L'){
                        left_out_cnt=1;
                        is_lastCardActive=true;
                    }
                    else if (intrested_type=='S'){
                        way=!way;
                        if (player_cnt-won_players<=2)
                            return;
                    }


                    stringstream s;
                    s<<"PUT "<<actual_player<<" 1 "<<last_card.get_str()<<" ";    /// HISTORY
                    string history_str=s.str();
                    for (int k=0; k<player_cnt; k++){
                        if (!players[k].robot && k!=actual_player)
                            players[k].history.push_back(history_str);
                    }

                    Next_Player();
                    return;
                }
                if (i==players[actual_player].cards.size()-1){
                    cout<<"Server_Put() hiba, nincs ilyen lapja a jatekosnak! Jatekos: "<<players[actual_player].client.id<<", lap: "<<recvbuff<<endl;
                    reset(clients);
                    return;
                }
            }
        } else {
            cout<<"Server_Put() hiba, nem rakhat ilyen lapot! Jatekos: "<<players[actual_player].client.id<<", lap: "<<recvbuff<<endl;
            reset(clients);
            return;
        }
    } else {
        cout<<recvbuff<<" "<<temp.size()<<endl;
        char temp_c=temp[0].get_type(); bool ok=true;
        if (temp.size()==2){
            if (temp[0].get_str()!=temp[1].get_str())
                ok=false;
        } else {
            for (size_t i=0; i<temp.size(); i++){
                if (temp[i].get_type()!=temp_c){
                    ok=false;
                    cout<<"A "<<i<<" "<<temp_c<<" "<<temp[i].get_type()<<endl;
                    break;
                }
            }
        }
        if (!ok){
            cout<<"Server_Put() hiba, nem rakhat le ilyen lapokat egyszerre! Jatekos: "<<players[actual_player].client.id<<", lapok: "<<recvbuff<<endl;
            reset(clients);
            return;
        } else if (!is_capable_card(temp[0].get_str())) {
            cout<<"Server_Put() hiba, nem rakhat le elsőnek ilyen lapot! Jatekos: "<<players[actual_player].client.id<<", lapok: "<<recvbuff<<endl;
            reset(clients);
            return;
        } else {
            draw_cards_cnt=0;

            stringstream s;
            s<<"PUT "<<actual_player<<" "<<temp.size()<<" ";    /// HISTORY

            for (size_t i=0; i<temp.size(); i++){
                for (size_t j=0; j<players[actual_player].cards.size(); j++){
                    if (temp[i].get_str()==players[actual_player].cards[j].get_str()){
                        players[actual_player].cards.erase(players[actual_player].cards.begin()+j);
                        dobopakli.push_back(temp[i]);
                        last_card.fullset(temp[0].get_str());
                        intrested_color=temp[0].get_color();
                        intrested_type=temp[0].get_type();
                        if (temp_c=='P')
                            draw_cards_cnt+=2;
                        if (temp_c=='P' || temp_c=='L')
                            is_lastCardActive=true;
                        if (temp_c=='L') {
                            left_out_cnt++;
                            if (player_cnt-won_players<=2)
                                left_out_cnt=1;
                        }
                        if (temp_c=='S')
                            way=!way;

                        s<<last_card.get_str()<<" ";    /// HISTORY

                        break;
                    }
                    if (j==players[actual_player].cards.size()-1){
                        cout<<"A "<<i<<" "<<temp_c<<" "<<temp[i].get_type()<<endl;
                        cout<<"Server_Put() hiba, nincs ilyen lapja a jatekosnak! Jatekos: "<<players[actual_player].client.id<<", lapok: "<<recvbuff<<endl;
                        reset(clients);
                        return;
                    }
                }
            }
            //if (temp_c=='L' && player_cnt-won_players<=2)
              //  return;
            string history_str=s.str();
                for (int k=0; k<player_cnt; k++){
                    if (!players[k].robot && k!=actual_player)
                        players[k].history.push_back(history_str); /// HISTORY
            }

            Next_Player();
            return;
        }
    }
}

void Game::Server_Put_robot() {  /// ROBOT DONE     HISTORY DONE
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Put_robot"<<endl;
    }

    for (size_t i=0; i<players[actual_player].cards.size(); i++){
        if ((intrested_color==players[actual_player].cards[i].get_color()  ||
            intrested_type==players[actual_player].cards[i].get_type()) && players[actual_player].cards[i].get_color()!='J'){
            dobopakli.push_back(players[actual_player].cards[i]);
            last_card=players[actual_player].cards[i];
            players[actual_player].cards.erase(players[actual_player].cards.begin()+i);
            intrested_color=last_card.get_color();
            intrested_type=last_card.get_type();
            if (intrested_type=='S' || intrested_type=='L'){
                if (player_cnt-won_players<=2){
                    return;
                }
                else {
                    if (intrested_type=='L'){
                        left_out_cnt=1;
                        is_lastCardActive=true;
                    }
                    else if (intrested_type=='S')
                        way=!way;
                }
            } else if (intrested_type=='P') {
                is_lastCardActive=true;
                draw_cards_cnt=2;
            }

            stringstream s;
            s<<"PUT "<<actual_player<<" 1 "<<last_card.get_str()<<" ";    /// HISTORY
            string history_str=s.str();
                for (int k=0; k<player_cnt; k++){
                    if (!players[k].robot && k!=actual_player)
                        players[k].history.push_back(history_str);
            }

            Next_Player();
            return;
        }
    }

    for (size_t i=0; i<players[actual_player].cards.size(); i++){
        if ('J'==players[actual_player].cards[i].get_color()){
            dobopakli.push_back(players[actual_player].cards[i]);
            last_card=players[actual_player].cards[i];
            players[actual_player].cards.erase(players[actual_player].cards.begin()+i);
            intrested_color='R';
            intrested_type='N';
            stringstream s;
            s<<"PUT "<<actual_player<<" 1 "<<last_card.get_str()<<" ";    /// HISTORY
            for (size_t j=0; j<players[actual_player].cards.size(); j++){
                if (players[actual_player].cards[j].get_color()!='J'){
                    intrested_color=players[actual_player].cards[j].get_color();
                    break;
                }
            }

            s<<intrested_color<<" "; /// HISTORY

            if (last_card.get_type()=='P'){
                is_lastCardActive=true;
                draw_cards_cnt=4;
            }


            string history_str=s.str();
                for (int k=0; k<player_cnt; k++){
                    if (!players[k].robot && k!=actual_player)
                        players[k].history.push_back(history_str);  /// HISTORY
            }

            Next_Player();
            return;
        }
    }
    Server_Cards_robot();
    Next_Player();
}

void Game::Server_PPut(char* recvbuff, std::vector<Client> &clients) { /// HISTORY DONE, Több lap egyszerre DONE (ellenőrzés szükséges)
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_PPut"<<endl;
    }

    vector<Card> temp;
    string cmd;
    stringstream s; s<<recvbuff;
    s>>cmd;
    Card card;
    s>>cmd;
    if (!(cmd.size()==2 && cmd[1]=='P')){
        cout<<"Server_Put() hiba, nem kert jol szint a jatekos! Jatekos: "<<players[actual_player].client.id<<", uzenet: "<<recvbuff<<endl;
        reset(clients);
    }
    if (cmd[0]=='J' && cmd[1]=='P'){
        for (size_t i=0; i<players[actual_player].cards.size(); i++){
            if (players[actual_player].cards[i].get_str()==cmd){
                players[actual_player].cards.erase(players[actual_player].cards.begin()+i);
                card.fullset(cmd);
                dobopakli.push_back(card);
                last_card=card;
                draw_cards_cnt+=4;
                is_lastCardActive=true;
                s>>cmd;

                stringstream s;
                s<<"PUT "<<actual_player<<" 1 "<<last_card.get_str()<<" "; /// HISTORY

                if (cmd[0]=='R' || cmd[0]=='G' || cmd[0]=='B' || cmd[0]=='Y'){
                    intrested_color=cmd[0];
                    intrested_type='N';

                    s<<intrested_color<<" ";        /// HISTORY

                } else {
                    cout<<"Server_PPut() hiba, nem kert jol szint a jatekos! Jatekos: "<<players[actual_player].client.id<<", uzenet: "<<recvbuff<<endl;
                    reset(clients);
                }

                string history_str=s.str();
                    for (int k=0; k<player_cnt; k++){
                        if (!players[k].robot && k!=actual_player)
                            players[k].history.push_back(history_str); /// HISTORY
                }

                Next_Player();
                return;
            }
            if (i==players[actual_player].cards.size()-1){
                cout<<"Server_PPut() hiba, nincs ilyen lapja a jatekosnak! Jatekos: "<<players[actual_player].client.id<<", lap: "<<temp[i].get_str()<<endl;
                reset(clients);
                return ;
            }
        }
    }
    while (true){
        if (cmd.size()==2){
            card.fullset(cmd);
            temp.push_back(card);
            if (s.eof() || temp.size()==8)
                break;
        } else {
            cout<<"Server_PPut() hiba, hibas uzenet, nincs ilyen lap! Jatekos: "<<players[actual_player].client.id<<", lap: "<<recvbuff<<endl;
            reset(clients);
            return;
        }
        s>>cmd;
    }
    if (temp.size()<=0){
        cout<<"Server_PPut() hiba, hibas uzenet, nem kuldott lapot! Jatekos: "<<players[actual_player].client.id<<", lap: "<<recvbuff<<endl;
        reset(clients);
        return;
    }
    else if (temp.size()==1){
        if (temp[0].get_type()=='P' && temp[0].get_color()!='J'){
            for (size_t i=0; i<players[actual_player].cards.size(); i++){
                if (players[actual_player].cards[i].get_str()==temp[0].get_str()){
                    players[actual_player].cards.erase(players[actual_player].cards.begin()+i);
                    dobopakli.push_back(temp[0]);
                    last_card.fullset(temp[0].get_str());
                    intrested_color=temp[0].get_color();
                    intrested_type=temp[0].get_type();
                    draw_cards_cnt+=2;
                    is_lastCardActive=true;

                    stringstream s;
                    s<<"PUT "<<actual_player<<" 1 "<<last_card.get_str()<<" ";    /// HISTORY
                    string history_str=s.str();
                        for (int k=0; k<player_cnt; k++){
                            if (!players[k].robot && k!=actual_player)
                                players[k].history.push_back(history_str);
                    }

                    Next_Player();
                    return;
                }
                if (i==players[actual_player].cards.size()-1){
                    cout<<"Server_PPut() hiba, nincs ilyen lapja a jatekosnak! Jatekos: "<<players[actual_player].client.id<<", lap: "<<recvbuff<<endl;
                    reset(clients);
                    return;
                }
            }
        } else {
            cout<<"Server_PPut() hiba, nem rakhat ilyen lapot! Jatekos: "<<players[actual_player].client.id<<", lap: "<<recvbuff<<endl;
            reset(clients);
            return;
        }
    } else {
        char temp_c='P'; bool ok=true;
        if (temp.size()==2){
            if (temp[0].get_str()==temp[1].get_str())
                ok=true;
        } else {
            for (size_t i=0; i<temp.size(); i++){
                if (temp[i].get_type()!=temp_c || temp[i].get_color()=='J'){
                    ok=false;
                    break;
                }
            }
        }
        if (!ok){
            cout<<"Server_PPut() hiba, nem rakhat le ilyen lapokat egyszerre! Jatekos: "<<players[actual_player].client.id<<", lapok: "<<recvbuff<<endl;
            reset(clients);
            return;
        } else {

            stringstream s;
            s<<"PUT "<<actual_player<<" "<<temp.size()<<" ";    /// HISTORY

            for (size_t i=0; i<temp.size(); i++){
                for (size_t j=0; j<players[actual_player].cards.size(); j++){
                    if (temp[i].get_str()==players[actual_player].cards[j].get_str()){
                        players[actual_player].cards.erase(players[actual_player].cards.begin()+j);
                        dobopakli.push_back(temp[i]);
                        last_card.fullset(temp[0].get_str());
                        intrested_color=temp[0].get_color();
                        intrested_type=temp[0].get_type();
                        draw_cards_cnt+=2;
                        is_lastCardActive=true;

                        s<<last_card.get_str()<<" ";  /// HISTORY

                        break;
                    }
                    if (j==players[actual_player].cards.size()-1){
                        cout<<"Server_PPut() hiba, nincs ilyen lapja a jatekosnak! Jatekos: "<<players[actual_player].client.id<<", lapok: "<<recvbuff<<endl;
                        reset(clients);
                        return;
                    }
                }
            }

            string history_str=s.str();
                for (int k=0; k<player_cnt; k++){
                    if (!players[k].robot && k!=actual_player)
                        players[k].history.push_back(history_str);  /// HISTORY
            }

            Next_Player();
            return;
        }
    }
}

bool Game::Server_PPut_robot() {  /// ROBOT DONE    HISTORY DONE
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_PPut_robot"<<endl;
    }

    for (size_t i=0; i<players[actual_player].cards.size(); i++) {
        if (((players[actual_player].cards[i].get_color()=='J' && intrested_color=='J') ||
              (last_card.get_color()!='J' && players[actual_player].cards[i].get_color()!='J')) && players[actual_player].cards[i].get_type()=='P'){
            dobopakli.push_back(players[actual_player].cards[i]);
            last_card=players[actual_player].cards[i];
            intrested_type='N';
            intrested_color='R';
            stringstream s;
            s<<"PUT "<<actual_player<<" 1 "<<last_card.get_str()<<" ";    /// HISTORY
            if (last_card.get_color()=='J')
                s<<intrested_color<<" ";
            for (size_t j=0; j<players[actual_player].cards.size(); j++){
                if (players[actual_player].cards[j].get_color()!='J'){
                    intrested_color=players[actual_player].cards[j].get_color();
                    break;
                }
            }
            players[actual_player].cards.erase(players[actual_player].cards.begin()+i);
            if (last_card.get_color()=='J')
                draw_cards_cnt+=4;
            else
                draw_cards_cnt+=2;
            is_lastCardActive=true;


            string history_str=s.str();
                for (int k=0; k<player_cnt; k++){
                    if (!players[k].robot && k!=actual_player)
                        players[k].history.push_back(history_str);
            }

            return true;
        }
    }
    return false;
}

void Game::Server_Win(std::vector<Client> &clients) {   /// HISTORY DONE
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Win"<<endl;
    }

        players[actual_player].won=true;
        char sendbuff[1024];
        strcpy(sendbuff,"WIN "); sendbuff[4]=num_to_char(won_players); sendbuff[5]=0;

        for (size_t i=0; i<points_name.size(); i++){
            if (points_name[i]==players[actual_player].client.id){
                points[i]+=won_players;
                break;
            }
        }
        won_players++;

        if (won_players>=player_cnt-1)
            state=ended;
        server_tcp_send_char(players[actual_player].client.socket, sendbuff);
        server_tcp_recv_char(players[actual_player].client.socket, sendbuff);

        stringstream s;
        s<<"WIN "<<actual_player<<" ";                  /// HISTORY
        string history_str=s.str();
            for (int k=0; k<player_cnt; k++){
                if (!players[k].robot && k!=actual_player)
                    players[k].history.push_back(history_str);
        }

        Next_Player();
        if (sendbuff[0]=='O' && sendbuff[1]=='K')
            return;
        cout<<"Server_Win() hiba, nem OK-zott gyozelem! Jatekos uzenete: "<<sendbuff<<endl;
        reset(clients);
}

void Game::Server_Win_robot(){  /// ROBOT DONE      HISTORY DONE
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_Win_robot"<<endl;
    }

    players[actual_player].won=true;
    won_players++;
    if (won_players>=player_cnt-1)
        state=ended;

    stringstream s;
    s<<"WIN "<<actual_player<<" ";                  /// HISTORY
    string history_str=s.str();
    for (int k=0; k<player_cnt; k++){
        if (!players[k].robot && k!=actual_player)
            players[k].history.push_back(history_str);
    }

    Next_Player();
}

void Game::Server_YourTurn(std::vector<Client> &clients) {   /// HISTORY (TRANZ)
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_YourTurn"<<endl;
    }

    if (!is_lastCardActive)
        Server_None(clients);
    else if (last_card.get_str()[1]=='P')
        Server_Plus(clients);
    else if (last_card.get_str()[1]=='L')
        Server_Off(clients);
    else {
        cout<<"Server_YourTurn() hiba, szerver oldali. Kritikus! Last card: "<<last_card.get_str()<<" True: "<<true<<", is_Lastcard: "<<is_lastCardActive<<endl;
        reset(clients);
    }

}

void Game::Server_YourTurn_robot() {    /// ROBOT DONE      HISTORY (TRANZ)
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
        cout<<"Server_YourTurn_robot"<<endl;
    }

    if (!is_lastCardActive)
        Server_None_robot();
    else if (last_card.get_str()[1]=='P')
        Server_Plus_robot();
    else if (last_card.get_str()[1]=='L')
        Server_Off_robot();
    else {
        cout<<"Server_YourTurn_robot() hiba, szerver oldali. Kritikus! Last card: "<<last_card.get_str()<<endl;
        //Sleep(10000);
        //reset();
    }

}

void Game::Server_Ingame(std::vector<Client> &clients) {
    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;

        cout<<"Server_Ingame"<<endl;
    }

    if (!players[actual_player].won){
        if (!players[actual_player].robot){
            if (players[actual_player].zeroCards())
                Server_Win(clients);
            else
                Server_YourTurn(clients);
        } else {                                    /// ROBOT
            if (players[actual_player].zeroCards())
                Server_Win_robot();
            else
                Server_YourTurn_robot();
        }
    } else {
        cout<<"Server_Ingame_error()"<<endl;
        Next_Player();
    }

    if (DEBUG_CMD_commands) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        cout<<time_ms1-time_ms2<<endl;
        time_ms2 = time_ms1;
    }
}

void Game::Server_Result(std::vector<Client> &clients) {
    ///cout<<"Server_Result"<<endl;
    if (DEBUG_CMD_game_count) {
        GetSystemTime(&time);
        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
        time_sec1=std::time(NULL);
        cout<<"Gametime: "<<time_ms1-time_ms2<<" ms, or: "<<time_sec1-time_sec2<<" sec. ID: "<<player_cnt*10+robot_cnt<<endl;
        time_sec2=time_sec1;
        time_ms2 = time_ms1;
    }
    for (int i=0; i<player_cnt; i++){
        if (!players[i].robot){
            char msg[20]; for (int j=0; j<20; j++) msg[j]=0;
            strcpy(msg,"RESULT ");
            char pos[10]; for (int j=0; j<10; j++) pos[j]=0;
            stringstream s;
            for (int j=0; j<player_cnt-robot_cnt; j++){
                if (points_name[j]==players[i].client.id){
                    cout<<players[i].client.id<<" "<<points[j];
                    s<<points[j];
                    s>>pos;
                    strcat(msg,pos);
                    server_tcp_send_char(players[i].client.socket, msg);
                }
            }
        }
    }
    //cout<<"Server_Result() hiba, szerver oldali. Kritikus! Last card: "<<last_card.get_str()<<endl;
    cout<<"Jatek vege! ID: "<<player_cnt*10+robot_cnt<<endl;
    reset(clients);
}

void Game::step(std::vector<Client> &clients) {
    if (active_players==player_cnt-robot_cnt){
        //cout<<"State: "<<state<<endl;
        if (state==wait)
            Server_getReadys(clients);
        else if (state==ready)
            Server_Data_send();
        else if (state==data_sent)
            Server_Data_ok(clients);
        else if (state==data_ok)
            Server_Ingame(clients);
        else if (state==ended)
            Server_End(clients);
        else if (state==stop)
            Server_Result(clients);
        else
            cout<<state<<endl;
    } else if (active_players!=0) {
        time_t t;
        if (std::time(NULL)>last_msg_time-1+DELAY) {
            last_msg_time=std::time(NULL);
            cout<<"Game ID: "<<player_cnt*10+robot_cnt<<", jatekosok szama: "<<active_players<<", true: "<<true<<", elindulasi feltetel: "<<(active_players==player_cnt-robot_cnt)<<endl;
        }
    }
}

int recive_data(Client &client, char* recvbuff){
    return server_tcp_recv_char(client.socket, recvbuff);
}

bool Game::addNewPlayer(Client &client, std::string id) {
    ///cout<<active_players<<" "<<player_cnt-robot_cnt<<" "<<(active_players<(player_cnt-robot_cnt))<<" "<<((active_players<(player_cnt-robot_cnt)) && (!ended_game))<<endl;
    if ((active_players<(player_cnt-robot_cnt)) && (!ended_game)){
        int ran=rand()%(player_cnt-robot_cnt-active_players);
        ///cout<<ran<<endl;
        for (int i=0; i<player_cnt; i++){
            if (!players[i].connected){
                if (ran>0) {
                    ran--;
                }
                else {
                    players[i].client=client;
                    players[i].client.id=id;
                    players[i].client.ingame=true;
                    players[i].connected=true;
                    players[i].data=false;
                    points_name[active_players]=id;
                    points[active_players]=0;
                    active_players++;
                    server_tcp_send_string(players[i].client.socket, "OK Szobahoz kapcsolodas sikerult!");
                    cout<<"Game room no: "<<player_cnt*10+robot_cnt<<", new Player connected: "<<players[i].client.id<<", player count: "<<active_players<<endl;

                    if (DEBUG_CMD_game_count) {
                        GetSystemTime(&time);
                        time_ms1 = (time.wSecond * 1000) + time.wMilliseconds;
                        //cout<<"Gametime reset!"<<endl;
                        time_ms2 = time_ms1;
                        time_sec1=std::time(NULL);
                        time_sec2=time_sec1;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

void addPlayer_to_Game(Client &client, std::vector<Game> &games) {
    char recvbuff[1024];
    if (recive_data(client, recvbuff)==-1){
        cout<<"addPlayer_to_Game::recive_data hiba! Nem erkezett uzenet!"<<std::endl;
        client.real_reset();
        return;
    }
    //cout<<recvbuff<<endl;
    stringstream s;
    s<<recvbuff;
    string cmd,id;
    int num;
    s>>cmd;
    if (cmd=="CONNECT") {
        s>>cmd;
        if (cmd=="ROOM") {
            s>>num>>cmd;
            for (size_t i=0; i<games[0].kliensek[0].size(); i++) {
                if (games[0].kliensek[0][i].id==cmd)
                    break;
                if (i==games[0].kliensek[0].size()-1){
                    server_tcp_send_string(client.socket, "REFUSED Hibas ID!");
                    client.real_reset();
                }
            }
            if (!games[roomNum_to_gameNum(num)].addNewPlayer(client, cmd)){
                cout<<"REFUSED A szoba betelt!"<<endl;
                server_tcp_send_string(client.socket, "REFUSED A szoba betelt!");
                client.real_reset();
            }
        } else if (cmd=="TEST") {
            s>>num>>cmd;
            for (size_t i=0; i<games[0].kliensek[0].size(); i++) {
                if (games[0].kliensek[0][i].id==cmd)
                    break;
                if (i==games[0].kliensek[0].size()-1){
                    client.real_reset();
                }
            }
            if (!games[roomNum_to_gameNum(num*11+10)].addNewPlayer(client, cmd)){
                cout<<"REFUSED A teszt szerver épp használatban van!"<<endl;
                server_tcp_send_string(client.socket, "REFUSED A teszt szerver epp hasznalatban van, probalj meg akar egy masikat!");
                client.real_reset();
            }

        } else {
            cout<<"REFUSED Hibas az uzenet!"<<endl;
            server_tcp_send_string(client.socket, "REFUSED Hibas az uzenet!");
            client.real_reset();
        }
    } else {
        cout<<"addPlayer_to_Game::recive_data hiba! Hibas uzenet erkezett!"<<std::endl;
        client.real_reset();
        return;
    }
}

void accept_clients(std::vector<Client> &clients, std::vector<Game> &games) {
    for (int i=0; i<client_cnt; i++){
        //cout<<"Listening: "<<i<<endl;
        if (clients[i].listening){
            switch (server_tcp_check_listening(clients[i].socket)) {
                case 0:
                    // Timed out, do whatever you want to handle this situation
                    //printf("\nServer: Timeout lor while waiting you retard client!...\n");
                    break;
                case -1:
                    // Error occurred, more tweaking here and the recvTimeOutTCP()...
                   break;
                default:
                    clients[i].socket = server_tcp_accept(clients[i].socket);
                    clients[i].listening=false;
                    clients[i].accepted=true;
                    clients[i].ingame=false;
                    ///cout<<clients[i].id<<endl;
                    cout<<"Accepted! "<<i<<" "<<endl;;//clients[i].socket.port<<endl;
                    addPlayer_to_Game(clients[i], games);
            }
        }

    }

}

void relisten_clients(std::vector<Client> &clients) {
    for (int i=0; i<client_cnt; i++){
        if (!(clients[i].accepted || clients[i].listening || clients[i].ingame)) {
            clients[i].socket=server_tcp_create_listening_socket();
            server_tcp_bind(clients[i].socket,2020+i);
            server_tcp_start_listening(clients[i].socket);
            clients[i].listening=true;
            cout<<"Listening: "<<i<<endl;
        }
    }
}

void next_steps(std::vector<Client> &clients, std::vector<Game> &games){
    int c=0;
    for (int i=2; i<=8; i++){
        for (int j=0; j<=i-2; j++){
            games[c].step(clients);
            c++;
        }
    }
    for (int i=1; i<=7; i++){
        games[c].step(clients);
        c++;
    }
}


























