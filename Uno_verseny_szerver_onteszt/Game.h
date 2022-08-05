#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Network.h"
#include "Server.h"
#include <time.h>

#define GAME_COUNT 200
#define DELAY 4

void card_cpy_str_to_char(std::string str, char* card);

bool is_starter_card(std::string str);

class Card {
    std::string str;
public:
    Card() { str="NN"; }

    void fullset(std::string card_str) {str=card_str;}

    char get_color() {return str[0];}

    char get_type() {return str[1];}

    void kiir() { std::cout<<str<<std::endl;}

    std::string get_str() {return str;}
};

class History {
public:
    std::vector<std::string> history;
};

class Player {
public:
    std::vector<Card> cards;
    Client client;
    bool won;
    bool robot;
    bool connected;
    bool data;
    std::vector<std::string> history;
    ///Hitory history

public:
    Player(){won=false; robot=false; connected=false; data=false; cards.resize(0); history.resize(0);}

    void addCard(Card card) {cards.push_back(card);}

    void setRobot() {robot=true; connected=true; data=false; won=false; history.resize(0);}

    void setPlayer() {robot=false; connected=false; data=false; won=false; history.resize(0);}

    bool get_data() {return data;}

    void cards_write() {
        for (size_t i=0; i<cards.size(); i++) std::cout<<cards[i].get_str()<<" ";
        std::cout<<std::endl;
    }

    bool zeroCards() {
        if (cards.size()==0)
            return true;
        return false;
    }
};

enum Game_state{
    wait, ready, data_sent, data_ok, ingame, ended, stop
};

class Game {


    std::vector<Player> players;
    std::vector<int> points;
    std::vector<std::string> points_name;
    std::list<Card> pakli;
    std::list<Card> dobopakli;

    Card last_card;
    char intrested_color, intrested_type;
    int draw_cards_cnt;
    int left_out_cnt;
public:
    int active_players;
    int player_cnt;
    int robot_cnt;
    int last_msg_time;
private:
    int won_players;
    int actual_player;
    int games_count;
    int games_max;
    bool is_lastCardActive;
    bool way;
    bool ended_game;
    Game_state state;
    int data_sent_cnt;
    LONG time_ms1;
    LONG time_ms2;
    SYSTEMTIME time;
    int time_sec1, time_sec2;


public:
    static std::vector<Client> *kliensek;

    Game(int room_num);

    void set_kliensek(std::vector<Client> &clients);

    void osztas();

    bool addNewPlayer(Client &client, std::string id);

    void Player_Draw(std::string command);

    void Player_Draw_robot();

    void Player_Put(std::string command);

    void Player_Put_robot();

    void Player_Ok();

    void Player_Bye();

    void Server_Data_send();

    void Server_Data_ok(std::vector<Client> &clients);

    void Server_Ingame(std::vector<Client> &clients);

    void Next_Player();

    void Server_YourTurn(std::vector<Client> &clients);

    void Server_YourTurn_robot();

    void shuffle_dobopakli(int remain, std::vector<Client> &clients);

    void Server_Cmd(char* sendbuff, int sft);

    void Server_Cards(std::vector<Client> &clients);

    void Server_Cards_robot();

    bool is_capable_card(std::string next);

    void Server_Put(char* recvbuff, std::vector<Client> &clients);

    void Server_Put_robot();

    void Server_PPut(char* recvbuff, std::vector<Client> &clients);

    bool Server_PPut_robot();

    void Server_None(std::vector<Client> &clients);

    void Server_None_robot();

    void Server_Plus(std::vector<Client> &clients);

    void Server_Plus_robot();

    void Server_Off(std::vector<Client> &clients);

    void Server_Off_robot();

    void Server_Win(std::vector<Client> &clients);

    void Server_Win_robot();

    void Server_End(std::vector<Client> &clients);

    void Server_Result(std::vector<Client> &clients);

    void Server_Error();

    void step(std::vector<Client> &clients);

    void reset(std::vector<Client> &clients);

    void shuffel_reset();

    void Server_getReadys(std::vector<Client> &clients);
};

void rendezett_pakli (std::vector<Card> keveresre);

void keveres(std::list<Card> &pakli);

void prepare(std::vector<Client> &clients, std::vector<Game> &games);

void accept_clients(std::vector<Client> &clients, std::vector<Game> &games);

void relisten_clients(std::vector<Client> &clients);

void next_steps(std::vector<Client> &clients, std::vector<Game> &games);

#endif // GAME_H_INCLUDED
