#include "Server.h"

#include <string.h>
#include "Network.h"

Server::Server()
{

    SV_Address = "127.0.0.1";
    SV_Port = "34567";
    uuid_generate(SV_ID);
}

Server::Server(std::string address, std::string port)
{
    SV_Address = address;
    SV_Port = port;
    uuid_generate(SV_ID);
}

Server::Server(std::string port)
{
    SV_Address = "127.0.0.1";
    SV_Port = port;
    uuid_generate(SV_ID);
}

Server::~Server()
{

}

returnCodes_t Server::run()
{
    struct sockaddr_storage address;
    socklen_t address_len;
    SV_Net.CreateSocket(SV_Address, SV_Port, SOCK_SERVER, &address, &address_len);

    NET_Type type;
    sockaddr_storage client;
    socklen_t client_len;

    SV_Net.ReceiveFrame(&type, &client, &client_len);


    if(type == CONNECTION_REQUEST)
    {
        log(LG_DEBUG, "Server::run Connection request received");
        SV_Net.SendFrame(CONNECTION_ACCEPTED, client, client_len);
    }
}

