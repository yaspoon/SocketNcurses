#include "Server.h"

#include <string.h>
#include "Network.h"
#include <sys/epoll.h>

Server::Server()
{
    SV_Address = "127.0.0.1";
    SV_Port = "34567";
}

Server::Server(std::string address, std::string port)
{
    SV_Address = address;
    SV_Port = port;
}

Server::Server(std::string port)
{
    SV_Address = "127.0.0.1";
    SV_Port = port;
}

Server::~Server()
{

}

returnCodes_t Server::run()
{
    //net = Network(SV_Address, SV_Port, MODE_SERVER);
    net.Setup(SV_Address, SV_Port, MODE_SERVER);

    char data;

    while(true)
    {

        Event tmp = net.getEvent();

        switch(tmp.type)
        {
            case Event::EVENT_NET:
                handleNetEvent(tmp);
                break;
            case Event::EVENT_KEY:
                handleKeyEvent(tmp);
                break;
        }

        sendUpdate();
    }
}

bool Server::addClient(std::string address, std::string port, int id)
{
    bool retVal = false;

    if((clients.size() + 1) <= 32)
    {
        SV_Client newCLient;
        newCLient.status = Event::EVENT_NET_CONNECTED;
        newCLient.frameNo = 0;
        newCLient.nextFrameNo = 0;
        newCLient.ackNo = 0;
        newCLient.id = id;
        log(LG_DEBUG, const_cast<char *>("Server::addClient Created Client[%d]"), id);

        clients[id] = newCLient;

        retVal = true;
    }

    return retVal;;
}

void Server::handleNetEvent(Event event)
{
    Event::Net_Event net = event.net;
    switch(net.net_type)
    {
        case Event::EVENT_NET_CONNECT:
            log(LG_DEBUG, const_cast<char *>("Server::run got EVENT_CONNECTION from %s:%s"), net.address.c_str(), net.port.c_str());
            addClient(net.address, net.port, event.id);
            Event conAck;
            conAck.type = Event::EVENT_NET;
            conAck.net.net_type = Event::EVENT_NET_CONNECT_ACK;
            conAck.net.address = net.address;
            conAck.net.port = net.port;
            conAck.id = event.id;
            this->net.sendEvent(conAck);
        break;
    }
}

void Server::handleKeyEvent(Event event)
{
    switch(event.key.key_type)
    {
        case Event::EVENT_KEY_DOWN:
        {
            switch(event.key.sym)
            {
                case Event::KEY_A:
                    clients[event.id].CL_player.x = clients[event.id].CL_player.x - 1;
                    log(LG_DEBUG, const_cast<char *>("Client[%d].CL_Player.x=%d"), event.id, clients[event.id].CL_player.x);
                break;
                case Event::KEY_W:
                    clients[event.id].CL_player.y = clients[event.id].CL_player.y - 1;
                    log(LG_DEBUG, const_cast<char *>("Client[%d].CL_Player.y=%d"), event.id, clients[event.id].CL_player.y);
                break;
                case Event::KEY_S:
                    clients[event.id].CL_player.y = clients[event.id].CL_player.y + 1;
                    log(LG_DEBUG, const_cast<char *>("Client[%d].CL_Player.y=%d"), event.id, clients[event.id].CL_player.y);
                break;
                case Event::KEY_D:
                    clients[event.id].CL_player.x = clients[event.id].CL_player.x + 1;
                    log(LG_DEBUG, const_cast<char *>("Client[%d].CL_Player.x=%d"), event.id, clients[event.id].CL_player.x);
                break;
            }
        }
        break;
    }
}

void Server::sendUpdate()
{
    Event update;
    update.type = Event::EVENT_GAMEUPDATE;

    for(int i = 0; i < clients.size(); i++)
    {
        //log(LG_DEBUG, "Sending update to Client[%d].id:%d c:%c x:%d y:%d", i, clients[i].id, clients[i].CL_player.character, clients[i].CL_player.x, clients[i].CL_player.y);
        update.id = clients[i].id;
        update.update.x = clients[i].CL_player.x;
        update.update.y = clients[i].CL_player.y;
        update.update.character = clients[i].CL_player.character;
        net.sendEvent(update);
    }
}


