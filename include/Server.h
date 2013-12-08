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
        uuid_t SV_ID;

        struct SV_Client
        {
            bool connected;
            uuid_t id;
            Player CL_player;
            struct sockaddr_storage CL_addr;
            socklen_t CL_addr_len;

            //Insert socket stuff here....
        };

        SV_Client clients[32];
        int clientCount;
        Network SV_Net;
};

#endif // SERVER_H
