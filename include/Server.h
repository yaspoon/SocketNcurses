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

        struct SV_Client
        {
            Event::Event_Net_type status;
            Player CL_player;
            int frameNo;
            int ackNo;
            int nextFrameNo;
            int id;

            //Insert socket stuff here....
        };

        std::map<int, SV_Client> clients;
        Network net;

        bool addClient(std::string address, std::string port, int id);
        bool handleFrame(int fd, NET_Frame frame, struct sockaddr_storage peer_addr, socklen_t peer_addr_len);
        void handleNetEvent(Event event);
        void handleKeyEvent(Event event);
        void sendUpdate();
};

#endif // SERVER_H
