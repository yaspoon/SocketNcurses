#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <sys/socket.h>

#include "Common.h"
#include <uuid/uuid.h>
#include "Player.h"
#include "Network.h"

class Client
{
    public:
        Client();
        Client(std::string address, std::string port);
        Client(std::string port);
        virtual ~Client();

        void run();
    protected:
    private:
        bool connect();
        std::string Server_Address;
        std::string Server_Port;
        int Server_ID;
        Network net;
        Player player;

        const static int CONNECTION_TIMEOUT = 2000; //2 seconds between conenction attempts
        const static int CONNECTION_ATTEMPTS = 5; //Amount of connection attempts before it gives up
        const static int MAXFPS = 60;

        void sendKey(Event::Event_Key_type type, Event::Key_Sym sym);
        void handleEvent(Event event);

};

#endif // CLIENT_H
