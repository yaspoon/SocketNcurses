#include "Server.h"

#include <string.h>
#include "Network.h"
#include <sys/epoll.h>

Server::Server()
{

    SV_Address = "127.0.0.1";
    SV_Port = "34567";
    uuid_generate(SV_ID);
    clientCount = 0;
}

Server::Server(std::string address, std::string port)
{
    SV_Address = address;
    SV_Port = port;
    uuid_generate(SV_ID);
    clientCount = 0;
}

Server::Server(std::string port)
{
    SV_Address = "127.0.0.1";
    SV_Port = port;
    uuid_generate(SV_ID);
    clientCount = 0;
}

Server::~Server()
{

}

returnCodes_t Server::run()
{
    net = Network(SV_Address, SV_Port, MODE_SERVER);
    net.Setup();

    while(true)
    {

    }
}

bool Server::addClient(uuid_t id, struct sockaddr_storage peer, socklen_t peer_len, int initialFrameNum)
{
    bool retVal = false;

    if((clientCount + 1) <= 32)
    {
        SV_Client newCLient;
        newCLient.connected = true;
        newCLient.frameNo = initialFrameNum;
        newCLient.nextFrameNo = 0;
        newCLient.ackNo = 0;
        memcpy(&(newCLient.CL_addr), &peer, sizeof(peer));
        newCLient.CL_addr_len = peer_len;
        mempcpy(newCLient.id, id, sizeof(id));

        clients[clientCount] = newCLient;
        clientCount += 1;

        retVal = true;
    }

    return retVal;;
}


