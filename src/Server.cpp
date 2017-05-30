#include "Server.h"

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "Network.h"
#include <sys/epoll.h>
#include <typeinfo>

#include "Buffer.h"
#include "WriteBuffer.h"

#include "Log.h"

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
    net.Setup(SV_Address, SV_Port, MODE_SERVER);

    char data;
    Timer timer;
    long int previousTime = 0;
    long int currentTime = 0;
    bool quit = false; //Don't quit, just keep looping ;)
    int MILLI_SECONDS_PER_SECOND = 1000;
    float timePerFrame = MILLI_SECONDS_PER_SECOND / MAXFPS;

	timer.start();
	previousTime = timer.getTime();
	currentTime = previousTime;

    while(!quit)
    {
	collectEvents(); /*Get all the events ready for the update*/

	processEvents();

	previousTime = currentTime;
	currentTime = timer.getTime();
        float delta = currentTime - previousTime;
	game.update(delta);

        sendUpdate();

        if(delta < timePerFrame)
        {
            usleep((timePerFrame - delta) * 1000);
        }
    }
}

returnCodes_t Server::processEvents()
{
	STUB << "just adding this so it compiles" << STUB_END;
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

	return OKAY;
}

returnCodes_t Server::collectEvents()
{
	STUB << "collectEvents doesn't do anything yet" << STUB_END;	
}

bool Server::addClient(std::string address, std::string port, int id)
{
	STUB << "Server::addClient STUB" << STUB_END;
	bool status = false;
	if((clients.size() + 1) <= 32)
	{
		SV_Client newCLient;
		newCLient.status = Event::EVENT_NET_CONNECTED;
		newCLient.frameNo = 0;
		newCLient.nextFrameNo = 0;
		newCLient.ackNo = 0;
		newCLient.id = id;
		STUB << "Server::addClient Created Client[" << id << "]" << STUB_END;

		//state.addPlayer(id);
		STUB << "STUB sending new client event to game" << STUB_END;

		clients[id] = newCLient;

		status = true;
	}

	return status;
}

void Server::handleNetEvent(Event event)
{
    Event::Net_Event net = event.net;
    switch(net.net_type)
    {
        case Event::EVENT_NET_CONNECT:
            DEBUG << "Server::run got EVENT_CONNECTION from " << net.address << ":" <<  net.port << DEBUG_END;
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

/*This should be handling Console commands*/
void Server::handleKeyEvent(Event event)
{
	STUB << "Server::handleKeyEvent STUB" << STUB_END;
}

void Server::sendUpdate()
{
	WriteBuffer update;
	update.writeEnum(Event::EVENT_GAMEUPDATE);

	STUB << "Server::sendUpdate isn't implemented!" << STUB_END;

	game.serialise(update);
	Event updateEvent(Event::EVENT_GAMEUPDATE, 0);
	updateEvent.setStream(update);

	//log(LG_DEBUG, "Update event entities %d", update.update.entities.size());

	for(int i = 0; i < clients.size(); i++)
	{
		updateEvent.setId(clients[i].id);
		//log(LG_DEBUG, "Sending update to Client[%d].id:%d", i, clients[i].id);
		net.sendEvent(updateEvent);
	}
}
