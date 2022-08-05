#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Uno_Network.h"
#define DEBUG true

enum Color {
    red='R',
    green='G',
    blue='B',
    yellow='Y',
    joker='J',
    none_c='N'
};


enum Type {
    null='0', one='1', two='2', three='3', four='4',
    five='5', six='6', seven='7', eight='8', nine='9',
    pls='P', swtch='S', left_out='L', clr_swtch='C',
    none_t='N'
};

class Card {
    Color color;
    Type type;
    std::string str;
public:
    Card() {color=none_c; type=none_t; str="NN";}

    void fullset(std::string card_str);

    char get_color() {return str[0];}

    char get_type() {return str[1];}

    string get_str() {return str;}
};


class Game {
    int player_cnt, ord_num;
    Card cards[108];
    bool way;

public:
    Game(char* recvbuff);

    void give_card(char* recvbuff, std::string card_str);

    bool put_card(char* recvbuff, char* sendbuff);

    void get_card(char* recvbuff);

    bool put_Pcard(char* recvbuff, char*  sendbuff);

};




#endif // GAME_H_INCLUDED
