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
        bool connectToServer();
        int CreateSocket(std::string address, std::string port);
    protected:
    private:
        std::string Server_Address;
        std::string Server_Port;
        uuid_t Client_ID;
        uuid_t Server_ID;
        Network CL_Net;

};

#endif // CLIENT_H
