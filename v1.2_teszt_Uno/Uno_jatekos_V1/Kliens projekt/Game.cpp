#include "Game.h"
using namespace std;

void Card::fullset (string card_str){
    str=card_str;
    ///color = card_str[0];
    ///type = card_str[1];
}

Game::Game (char* recvbuff) { /// DATA <játekosok szama> <sorszám a játékosnak> <kezdõ lap> 7x<lapok>
    stringstream s;
    s << recvbuff;
    string chk;
    string card_str;

    s >> chk;
    if (chk=="DATA")
        cout<< "REMEK!" << endl;
    s >> player_cnt >> ord_num;
    s >> card_str;          /// Kezdő lap
    for (int i=0; i<7; i++){
        s >> card_str;
        cards[i].fullset(card_str);
    }
    way=true;
}

bool Game::put_card(char* recvbuff, char* sendbuff) {
    stringstream t;
    t << recvbuff;
    char intrested_type, intrested_color;
    char temp[1024];
    sendbuff[0]='P'; sendbuff[1]='U'; sendbuff[2]='T'; sendbuff[3]=' ';
    t>>temp>>temp>>temp>>intrested_color>>intrested_type;
    for (int i=0; i<108; i++) {
        if (cards[i].get_str()!="NN"){
            if ((cards[i].get_color()==intrested_color || cards[i].get_type()==intrested_type) && cards[i].get_color() != 'J'){
                sendbuff[4]=cards[i].get_color();
                sendbuff[5]=cards[i].get_type();
                sendbuff[6]=0;
                cards[i].fullset("NN");
                return true;
            }
        }
    }
    for (int i=0; i<108; i++) {
        if (cards[i].get_color()=='J'){
            sendbuff[4]=cards[i].get_color();
            sendbuff[5]=cards[i].get_type();
            sendbuff[6]=' '; sendbuff[7]='R'; sendbuff[8]=0;
            for (int j=0; j<108; j++){
                if (cards[j].get_str()[0]=='R' || cards[j].get_str()[0]=='G' || cards[j].get_str()[0]=='B' || cards[j].get_str()[0]=='Y'){
                    sendbuff[7]=cards[j].get_str()[0];
                    j=108;
                }
            }
            cards[i].fullset("NN");
            return true;
        }
    }
    return false;
}

void Game::get_card(char* recvbuff) {

    stringstream a;
    a<<recvbuff;
    char temp[1024];
    a>>temp;    /// CARD(S) ...
    while (true) {
        a>>temp;        /// <lap>

        for (int i=0; i<108; i++){
            if (cards[i].get_str()=="NN"){
                cards[i].fullset(temp);
                break;
            }
        }

        if (a.eof()){
            //cout<<"Vege"<<endl;
            break;
        }
    }
}

bool Game::put_Pcard(char* recvbuff, char* sendbuff) {
    stringstream t;
    t << recvbuff;
    char last_card_type, last_card_color;
    char temp[1024];
    sendbuff[0]='P'; sendbuff[1]='U'; sendbuff[2]='T'; sendbuff[3]=' ';
    t>>temp>>temp>>temp>>last_card_color>>last_card_type;
    for (int i=0; i<108; i++) {
        if (((cards[i].get_color()=='J' && last_card_color=='J') || (last_card_color!='J' && cards[i].get_color()!='J')) && cards[i].get_type()=='P'){
            sendbuff[4]=cards[i].get_color();
            sendbuff[5]=cards[i].get_type();
            sendbuff[6]=0;
            if (sendbuff[4]=='J'){
                sendbuff[6]=' '; sendbuff[7]='R'; sendbuff[8]=0;
                for (int j=0; j<108; j++){
                    if (cards[j].get_str()[0]=='R' || cards[j].get_str()[0]=='G' || cards[j].get_str()[0]=='B' || cards[j].get_str()[0]=='Y'){
                        sendbuff[7]=cards[j].get_str()[0];
                        j=108;
                    }
                }
            }
            cards[i].fullset("NN");
            return true;
        }
    }
    return false;
}

void Game::give_card(char* recvbuff, string card_str) {
    char clr, typ;
    char temp[1024];
    stringstream s;
    s << recvbuff;
    s >>temp>>temp>>temp>>clr>>typ;

    for (int i=0; i<108; i++){
        if (cards[i].get_str()!="NN"){
            if (cards[i].get_color()==clr || cards[i].get_type()==typ){
                card_str=cards[i].get_str();
                return ;
            }
        }
    }

    }
