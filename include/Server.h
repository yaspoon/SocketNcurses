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
            int frameNo;
            int ackNo;
            int nextFrameNo;

            //Insert socket stuff here....
        };

        SV_Client clients[32];
        int clientCount;
        Network SV_Net;

        bool addClient(uuid_t id, struct sockaddr_storage peer, socklen_t peer_len, int initialFrameNum);
        bool handleFrame(int fd, NET_Frame frame, struct sockaddr_storage peer_addr, socklen_t peer_addr_len);
        bool sendACK(int fd, int ack_num, struct sockaddr_storage peer_addr, socklen_t peer_addr_len);
        bool sendFrame(int fd, NET_Frame frame, struct sockaddr_storage peer_addr, socklen_t peer_addr_len);
};

#endif // SERVER_H
