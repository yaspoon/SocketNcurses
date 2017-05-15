#include "Network.h"
#include "Common.h"
#include <string.h>

#include <sys/epoll.h>
#include <ncurses.h>

#include "ReadBuffer.h"
#include "WriteBuffer.h"
#include "Frame.h"

Network::Network()
{
    srand48_r((long int)time(NULL), &drand);
    setup = false;
    status = Event::EVENT_NET_DISCONNECTED;
    nextId = 0;
    pthread_mutex_init(&mutex, NULL);
}

Network::Network(std::string address, std::string port, NET_Mode mode)
{
    srand48_r((long int)time(NULL), &drand);
    setup = false;
    status = Event::EVENT_NET_DISCONNECTED;
    nextId = 0;
    pthread_mutex_init(&mutex, NULL);
}

Network::~Network()
{
    //dtor
}

bool Network::Setup(std::string address, std::string port, NET_Mode inMode)
{
    bool retVal = false;
    if(!setup)
    {
        if(CreateSocket(address, port, inMode) == true)
        {
            if(pthread_create(&runThread, NULL, run, (void*)this) == 0)
            {
                retVal = true;
                setup = true;
            }
            else
            {
                log(LG_ERROR, const_cast<char *>("Network::Setup Failed to create thread"));
            }
        }
        else
        {
            log(LG_ERROR, const_cast<char *>("Network::Setup Failed to create socket"));
        }
    }

    return retVal;
}

bool Network::SendFrame(int fd, Frame frame, sockaddr_storage peer, socklen_t peer_len)
{
	bool retVal = false;
	WriteBuffer buf;
	frame.serialise(buf);
	buf.flush();
	size_t frameSize = buf.totalBytes();
	uint8_t *data = buf.bytes();
	statistics.addBytes(frameSize, STAT_OUT);


	if(!sendto(fd, &data, frameSize, 0, (struct sockaddr*) &peer, peer_len) == frameSize) {
		log(LG_ERROR, const_cast<char *>("Network::SendFrame Failed to write frame ERROR:%s"), strerror(errno));
	}

	return retVal;
}

int Network::ReceiveFrame(int fd, Buffer &frame, struct sockaddr_storage *peer, socklen_t *peer_len)
{
	static uint8_t buf[MAX_NETWORK_BUFFER_BYTES];
	*peer_len = sizeof(struct sockaddr_storage);

	int nread = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)peer, peer_len);

	if(nread != -1) {
		//log(LG_DEBUG, "Network::ReceiveFrame Read %d bytes frame from socket", nread);
	} else {
		log(LG_ERROR, const_cast<char *>("Network::ReceiveFrame Failed to read from socket reason:%s"), strerror(errno));
	}

	frame.fillFromBuffer(buf, nread);

	return nread;

}

bool Network::CreateSocket(std::string address, std::string port, NET_Mode netType, int id)
{
    bool retVal = false;
    struct addrinfo hints;
    struct addrinfo *info;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int ret = -1;

    if(netType == MODE_SERVER)
    {
        ret = getaddrinfo(NULL, port.c_str(), &hints, &info);
    }
    else if(netType == MODE_CLIENT)
    {
        ret = getaddrinfo(address.c_str(), port.c_str(), &hints, &info);
    }

    if(ret == 0)
    {
        log(LG_DEBUG, const_cast<char *>("Got Address info, continuing"));

        struct addrinfo* rp = NULL;

        for(rp = info; rp != NULL; rp = rp->ai_next)
        {
            int tmpSfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            log(LG_DEBUG, const_cast<char *>("Creating Socket"));

            if(tmpSfd != -1)
            {
                log(LG_DEBUG, const_cast<char *>("Created Socket successfully"));
                if(netType == MODE_CLIENT)
                {
                    sfd = tmpSfd;
                    log(LG_DEBUG, const_cast<char *>("Network::Created socket successfully ready to send as client"));
                    connection tmp = connections[id];
                    memcpy(&tmp.peer_addr, rp->ai_addr, sizeof(struct sockaddr));
                    tmp.peer_addr_len = rp->ai_addrlen;
                    connections[id] = tmp;
                    retVal = true;
                    break;
                }
                else if(netType == MODE_SERVER)
                {
                    if(bind(tmpSfd, rp->ai_addr, rp->ai_addrlen) == 0)
                    {
                        sfd = tmpSfd;
                        log(LG_DEBUG, const_cast<char *>("Network::CreateSocket Bound socket for listening as server"));
                        retVal = true;
                        break;
                    }
                }
            }
        }

        freeaddrinfo(info);
    }
    else
    {
        log(LG_ERROR, const_cast<char *>("Network::CreateSocket Failed to get Address info Error:%s"), gai_strerror(ret));
    }

    return retVal;
}

void* Network::run( void* argument)
{
    Network *net = static_cast<Network*>(argument);
    log(LG_DEBUG, const_cast<char *>("THREAD::Network::run Added socket fd to epoll fine"));
    struct epoll_event ev;
    struct epoll_event events[10];
    int epfd;

    ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
    ev.data.fd = net->sfd;
    epfd = epoll_create(10);

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, net->sfd, &ev) == 0)
    {

        while(true)
        {

            //log(LG_DEBUG, "THREAD::Network::run Polling for network activity");
            int nfds = epoll_wait(epfd, events, 10, -1);

            if(nfds > 0)
            {
                //log(LG_DEBUG, "THREAD::Network::run Events received after polling YAY");

                for(int i = 0; i < nfds; i++)
                {
                    ReadBuffer buf;
		    Frame frame;
                    struct sockaddr_storage peer_addr;
                    socklen_t peer_addr_len = sizeof(struct sockaddr_storage);

		    int nread = ReceiveFrame(events[i].data.fd, buf, &peer_addr, &peer_addr_len);

                    net->statistics.addBytes(nread, STAT_IN);

                    char host[NI_MAXHOST];
                    char serv[NI_MAXSERV];

                    getnameinfo((struct sockaddr*)&peer_addr, peer_addr_len, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV);

		    if(frame.serialise(buf))
		    {
			    if(frame.getType() != Frame::ACK)
			    {
				net->sendACK(net->sfd, frame.getNum(), peer_addr, peer_addr_len);
			    }

			    switch(frame.getType())
			    {
				    case Frame::CONNECTION_REQUEST:
				{
				    log(LG_DEBUG, const_cast<char *>("Created new connection from %s:%s"), host, serv);
				    connection tmp;
				    std::string address(host);
				    std::string port(serv);
				    tmp.peer_addr = peer_addr;
				    tmp.peer_addr_len = peer_addr_len;
				    tmp.status = Event::EVENT_NET_CONNECT;
				    tmp.id = net->findId(address, port);
				    tmp.address = address;
				    tmp.port = port;

				    if(tmp.id == -1)
				    {
					tmp.id = net->nextId;
					net->nextId = net->nextId + 1;
				    }

				    log(LG_DEBUG, const_cast<char *>("New connection id:%d"), tmp.id);

				    net->connections[tmp.id] = tmp;

				    Event newEvent;
				    newEvent.type = Event::EVENT_NET;
				    newEvent.net.net_type = Event::EVENT_NET_CONNECT;
				    newEvent.net.address = address;
				    newEvent.net.port = port;
				    newEvent.id = tmp.id;

				    net->addEvent(newEvent);
				}
				break;
				case Frame::CONNECTION_REQUEST_ACK:
				{
				    Event tmp;
				    tmp.type = Event::EVENT_NET;
				    tmp.net.address = std::string(host);
				    tmp.net.port = std::string(serv);
				    tmp.net.net_type = Event::EVENT_NET_CONNECT_ACK;
				    tmp.id = net->findId(tmp.net.address, tmp.net.port);
				    net->addEvent(tmp);//.pushBack(tmp);
				    net->setStatus(Event::EVENT_NET_CONNECTED);
				}
				break;
				case Frame::CONNECTION_ACCEPTED:
				{
				    Event tmp;
				    tmp.type = Event::EVENT_NET;
				    tmp.net.net_type = Event::EVENT_NET_CONNECTED;
				    net->addEvent(tmp);//.pushBack(tmp);
				    log(LG_DEBUG, const_cast<char *>("THREAD::Network::run Connection accepted"));
				}
				break;
				case Frame::DATA:
				{
					//This should really be push up into the event layer and it can figure out what to do with it!
					log(LG_DEBUG, const_cast<char *>("STUB: THREAD::Network::run: data network frame unhandled"));
					std::cout << "host:" << host << " serv:" << serv << std::endl;
					/*int id = findId(address, port);
					Event tmp(EVENT_NET_DATA, id);
					tmp.setStream(buf);
					net->addEvent(tmp);*/

				}
				    break;
				break;
				case Frame::ACK:
				    //log(LG_DEBUG, "THREAD::Network::run Got ack frame");
				break;
				default:
				    log(LG_ERROR, const_cast<char *>("THREAD::Network::run Couldn't find frame type :("));
				    break;
			    }
		    } else {
			    std::cout << "Failed to deserialize Frame ignoring" << std::endl;
		    }
                }

            }
            else if(nfds == 0)
            {
                log(LG_DEBUG, const_cast<char *>("THREAD::Network::run No events"));
            }
            else if(nfds == -1)
            {
                log(LG_ERROR, const_cast<char *>("THREAD::Network::run epoll_wait returned error: %s"), strerror(errno));
            }

        }
    }
}

bool Network::sendACK(int fd, int ack_num, struct sockaddr_storage peer_addr, socklen_t peer_addr_len)
{
    bool retval = false;

    Frame frame(Frame::ACK, ack_num);
    retval = SendFrame(fd, frame, peer_addr, peer_addr_len);

    return retval;
}

void Network::handleConnectionRequest(int frameNum, sockaddr_storage peer_addr, socklen_t peer_addr_len)
{
    std::string address;
    std::string port;
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((sockaddr*)&peer_addr, peer_addr_len, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV);
    address = std::string(host);
    port = std::string(serv);

    log(LG_DEBUG, const_cast<char *>("Created new connection from %s:%s"), host, serv);
    connection tmp;
    tmp.frame_num = frameNum;
    tmp.peer_addr = peer_addr;
    tmp.peer_addr_len = peer_addr_len;
    tmp.status = Event::EVENT_NET_CONNECT;
    tmp.id = findId(address, port);
    tmp.address = address;
    tmp.port = port;

    if(tmp.id == -1)
    {
        tmp.id = nextId;
        nextId = nextId + 1;
    }

    connections[tmp.id] = tmp;

    Event newEvent;
    newEvent.type = Event::EVENT_NET;
    newEvent.net.net_type = Event::EVENT_NET_CONNECT;
    newEvent.net.address = address;
    newEvent.net.port = port;
    newEvent.id = tmp.id;

    eventList.push_back(newEvent);

}

void Network::sendEvent(Event event)
{
     switch(event.type)
    {
        case Event::EVENT_NET:
        {
            handleNetEvent(event);
        }
            break;
        case Event::EVENT_KEY:
        {
            handleKeyEvent(event);
        }
            break;
        case Event::EVENT_GAMEUPDATE:
        {
            handleGameUpdateEvent(event);
        }
            break;
    }

}

Event Network::getEvent(void)
{
    Event tmp;
    if(eventList.size() > 0 && pthread_mutex_lock(&mutex) == 0)
    {
        tmp = eventList.front();
        eventList.pop_front();
        pthread_mutex_unlock(&mutex);
    }

    return tmp;
}

Event::Event_Net_type Network::getStatus()
{
    return status;
}

void Network::handleNetEvent(Event event)
{
    Frame frame;
    Event::Net_Event net = event.net;

    switch(net.net_type)
    {
    case Event::EVENT_NET_CONNECT:
        {
            log(LG_DEBUG, const_cast<char *>("Network::sendEvent Event::CONNECT, sending connection request to %s:%s"), net.address.c_str(), net.port.c_str());
            //if(!setup)
            {
                log(LG_DEBUG, const_cast<char *>("Client ID set to %d"), nextId);
                event.id = 0;//nextId;
                //nextId++;
            }

            Setup(net.address, net.port, MODE_CLIENT);
            connection tmp;
            tmp = connections[event.id];
            tmp.id = event.id;
            tmp.address = net.address;
            tmp.port = net.port;
            tmp.status = Event::EVENT_NET_CONNECT;
            connections[event.id] = tmp;
            frame.setType(Frame::CONNECTION_REQUEST);
        }
        break;
    case Event::EVENT_NET_CONNECT_ACK:
        {
            log(LG_DEBUG, const_cast<char *>("Network::sendEvent Event::EVENT_NET_CONNECT_ACK event received"));
            frame.setType(Frame::CONNECTION_REQUEST_ACK);
        }
        break;
    default:
        log(LG_ERROR, const_cast<char *>("ERROR:Network::sendEvent unknown event type"));
    }

    connection to = connections[event.id];

    SendFrame(sfd, frame, to.peer_addr, to.peer_addr_len);
}

void Network::handleKeyEvent(Event event)
{
    Frame frame;
    frame.setType(Frame::DATA);
    frame.setNum(0);
    Event::Key_Event key = event.key;

    log(LG_DEBUG, const_cast<char *>("STUB: handleKeyEvent, event handling commented out for now\n"));

    /*if(key.key_type == Event::EVENT_KEY_DOWN)
    {
        frame.body.data.data_type = DATA_KEY_DOWN;
        frame.body.data.key = key.sym;
    }
    else if(key.key_type == Event::EVENT_KEY_UP)
    {
        frame.body.data.data_type = DATA_KEY_UP;
        frame.body.data.key = key.sym;
    }*/

    connection to = connections[event.id];

    SendFrame(sfd, frame, to.peer_addr, to.peer_addr_len);
}

void Network::handleGameUpdateEvent(Event event)
{
    Frame frame;
    frame.setType(Frame::DATA);
    frame.setNum(0);

    log(LG_DEBUG, const_cast<char *>("STUB: handleGameUpdateEvent, event handling commented out for now\n"));
    /*frame.body.data.data_type = DATA_GAMEUPDATE;

    int size = event.update.entities.size() > 8 ? 8 : event.update.entities.size();

    frame.body.data.numEnts = size;

    for(int i = 0; i < size; i++)
    {
        frame.body.data.ents[i] = event.update.entities[i];
    }*/

    connection to = connections[event.id];

    SendFrame(sfd, frame, to.peer_addr, to.peer_addr_len);
}

void Network::setStatus(Event::Event_Net_type newStatus)
{
    status = newStatus;
}

int Network::findId(std::string address, std::string port)
{
    int retVal = -1;

    for(int i = 0; i < connections.size(); i ++)
    {
        if(connections[i].address == address && connections[i].port == port)
        {
            retVal = connections[i].id;
        }
    }

    return retVal;
}

void Network::addEvent(Event event)
{
    if(pthread_mutex_lock(&mutex) == 0)
    {
        eventList.push_back(event);
        pthread_mutex_unlock(&mutex);
    }
    else
    {
        log(LG_DEBUG, const_cast<char *>("Failed to lock mutex :("));
    }

}

NetworkStatistics Network::getStatistics()
{
    return statistics;
}
