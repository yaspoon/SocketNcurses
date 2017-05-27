#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include "Common.h"
#include "Player.h"
#include "Network.h"

#include "Timer.h"
#include "Game.h"



class Server
{
    public:
        Server();
        Server(std::string address, std::string port);
        Server(std::string port);
        virtual ~Server();

        returnCodes_t run();
        bool startNetwork();
    protected:
    private:
        std::string SV_Address; //Servers address to bind to
        std::string SV_Port;
        const static int MAXFPS = 60;

	Game game;

        struct SV_Client
        {
            Event::Event_Net_type status;
            int frameNo;
            int ackNo;
            int nextFrameNo;
            int id;

            //Insert socket stuff here....
        };

        std::map<int, SV_Client> clients;
        Network net;

        bool addClient(std::string address, std::string port, int id);
        void handleNetEvent(Event event);
        void handleKeyEvent(Event event);
        void sendUpdate();
	returnCodes_t collectEvents();
};
#endif // SERVER_H
