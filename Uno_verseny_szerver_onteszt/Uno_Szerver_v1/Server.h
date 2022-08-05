#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "Network.h"
//#include "Game.h"



class Client {
public:
    SOCKET socket;
    bool listening, accepted, ingame;
    std::string id;

public:
    Client() {listening=false; accepted=false; ingame=false;}

    void reset() {closesocket(socket); listening=false; accepted=false; ingame=false; id.resize(0);}

    void real_reset() {closesocket(socket); listening=false; accepted=false; ingame=false;}

    void shuffel_reset() {accepted=true; ingame=true;}


};

class Test {
public:
    std::vector<Client> *almafa;

    Test(std::vector<Client> &alma) {
        almafa = &alma;
        almafa[0][0].listening=false;
        std::cout<<alma[0].listening<<std::endl;
    }

};

#endif // SERVER_H_INCLUDED
