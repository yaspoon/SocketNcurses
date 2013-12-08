#include "Client.h"

#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>


Client::Client()
{
    Server_Address = "127.0.0.1";
    Server_Port = "34567";
    uuid_generate(Client_ID);
    char id[37];
    uuid_unparse(Client_ID, id);
    log(LG_DEBUG, "Client ID:%s", id);
}

Client::Client(std::string address, std::string port)
{
    Server_Address = address;
    Server_Port = port;
    uuid_generate(Client_ID);
    char id[37];
    uuid_unparse(Client_ID, id);
    log(LG_DEBUG, "Client ID:%s", id);
}

Client::Client(std::string port)
{
    Server_Address = "127.0.0.1";
    Server_Port = port;
    uuid_generate(Client_ID);
    char id[37];
    uuid_unparse(Client_ID, id);
    log(LG_DEBUG, "Client ID:%s", id);
}

Client::~Client()
{
    uuid_clear(Client_ID);
}

void Client::run()
{
    if(connectToServer())
    {

    }
    else
    {
        log(LG_ERROR, "Failed to connect to server at %s port %s", Server_Address.c_str(), Server_Port.c_str());
    }
}

bool Client::connectToServer()
{
    int ret;
    bool retVal = false;

    CL_Net.ConnectTo(Server_Address, Server_Port, SOCK_CLIENT);



    return retVal;
}
