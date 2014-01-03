#include "Network.h"
#include "Common.h"
#include <string.h>

#include <sys/epoll.h>
#include <ncurses.h>
Network::Network()
{
    setup = false;
    status = Event::EVENT_NET_DISCONNECTED;
    nextId = 0;
    pthread_mutex_init(&mutex, NULL);
}

Network::Network(std::string address, std::string port, NET_Mode mode)
{
    setup = false;
    status = Event::EVENT_NET_DISCONNECTED;
    nextId = 0;
    pthread_mutex_init(&mutex, NULL);
}

Network::~Network()
{
    //dtor
}

bool Network::Setup(std::string address, std::string port, NET_Mode inMode, int id)
{
    bool retVal = false;
    if(!setup)
    {
        if(CreateSocket(address, port, inMode, id) == true)
        {
            if(pthread_create(&runThread, NULL, run, (void*)this) == 0)
            {
                retVal = true;
                setup = true;
            }
            else
            {
                log(LG_ERROR, "Network::Setup Failed to create thread");
            }
        }
        else
        {
            log(LG_ERROR, "Network::Setup Failed to create socket");
        }
    }

    return retVal;
}

size_t Network::sizeofFrame(NET_Frame frame)
{
    size_t retVal = 0;

    size_t body = sizeof(NET_Frame_Body);
    size_t header = sizeof(NET_Frame) - body; //Get size of everything but the body

    switch(frame.type)
    {
    case ACK:
        retVal = header;
        break;
    case CONNECTION_REQUEST:
        retVal = header;
        break;
    case CONNECTION_REQUEST_ACK:
            retVal = header;
        break;
    case CONNECTION_ACCEPTED:
        retVal = header;
        break;
    case DATA:
        retVal = header + body;//sizeof(frame.body.data);
        break;
    default:
        retVal = header + body;
    }

    return retVal;
}

bool Network::SendFrame(int fd, NET_Frame frame, sockaddr_storage peer, socklen_t peer_len)
{
    bool retVal = false;
    size_t frameSize = sizeofFrame(frame);

    if(sendto(fd, &frame, frameSize, 0, (struct sockaddr*) &peer, peer_len) == frameSize)
    {
        if(frame.type != ACK)
        {
            //log(LG_DEBUG, "Network::SendFrame Wrote %d bytes successfully, waiting for ack", frameSize);
        }
        else
        {
            //log(LG_DEBUG, "Network::SendFrame Wrote %d bytes ack frame successfully", frameSize);
        }

    }
    else
    {
        log(LG_ERROR, "Network::SendFrame Failed to write frame");
    }



    return retVal;
}

int Network::ReceiveFrame(int fd, NET_Frame *frame, struct sockaddr_storage *peer, socklen_t *peer_len)
{
    *peer_len = sizeof(struct sockaddr_storage);

    int nread = recvfrom(fd, frame, sizeof(NET_Frame), 0, (struct sockaddr*)peer, peer_len);

    if(nread != -1)
    {
        //log(LG_DEBUG, "Network::ReceiveFrame Read %d bytes frame from socket", nread);

    }
    else
    {
        log(LG_ERROR, "Network::ReceiveFrame Failed to read from socket reason:%s", strerror(errno));
    }

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
        log(LG_DEBUG, "Got Address info, continuing");

        struct addrinfo* rp = NULL;

        for(rp = info; rp != NULL; rp = rp->ai_next)
        {
            int tmpSfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            log(LG_DEBUG, "Creating Socket");

            if(tmpSfd != -1)
            {
                log(LG_DEBUG, "Created Socket successfully");
                if(netType == MODE_CLIENT)
                {
                    sfd = tmpSfd;
                    log(LG_DEBUG, "Network::Created socket successfully ready to send as client");
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
                        log(LG_DEBUG, "Network::CreateSocket Bound socket for listening as server");
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
        log(LG_ERROR, "Network::CreateSocket Failed to get Address info Error:%s", gai_strerror(ret));
    }

    return retVal;
}

void* Network::run( void* argument)
{
    Network *net = static_cast<Network*>(argument);
    log(LG_DEBUG, "THREAD::Network::run Added socket fd to epoll fine");
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
                    NET_Frame frame;
                    struct sockaddr_storage peer_addr;
                    socklen_t peer_addr_len = sizeof(struct sockaddr_storage);

                    int nread = recvfrom(events[i].data.fd, &frame, sizeof(frame), 0, (struct sockaddr*)&peer_addr, &peer_addr_len);

                    char host[NI_MAXHOST];
                    char serv[NI_MAXSERV];

                    getnameinfo((struct sockaddr*)&peer_addr, peer_addr_len, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV);

                    //log(LG_DEBUG, "THREAD::Network::run Received %d bytes from %s:%s", nread, host, serv);

                    if(frame.type != ACK)
                    {
                        net->sendACK(net->sfd, frame.num, peer_addr, peer_addr_len);
                    }

                    switch(frame.type)
                    {
                        case CONNECTION_REQUEST:
                        {
                            log(LG_DEBUG, "Created new connection from %s:%s", host, serv);
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

                            log(LG_DEBUG, "New connection id:%d", tmp.id);

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
                        case CONNECTION_REQUEST_ACK:
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
                        case CONNECTION_ACCEPTED:
                        {
                            Event tmp;
                            tmp.type = Event::EVENT_NET;
                            tmp.net.net_type = Event::EVENT_NET_CONNECTED;
                            net->addEvent(tmp);//.pushBack(tmp);
                            log(LG_DEBUG, "THREAD::Network::run Connection accepted");
                        }
                        break;
                        case DATA:
                            {
                                Event tmp;
                                switch(frame.body.data.data_type)
                                {
                                    case DATA_KEY_DOWN:
                                    {
                                        tmp.type = Event::EVENT_KEY;
                                        tmp.net.address = std::string(host);
                                        tmp.net.port = std::string(serv);
                                        tmp.net.net_type = Event::EVENT_NET_CONNECT_ACK;
                                        tmp.id = net->findId(tmp.net.address, tmp.net.port);
                                        tmp.key.key_type = Event::EVENT_KEY_DOWN;
                                        log(LG_DEBUG, "Network::run Got %c key from %s:%s", frame.body.data.key, tmp.net.address.c_str(), tmp.net.port.c_str());
                                        switch(frame.body.data.key)
                                        {
                                            case 'w':
                                                tmp.key.sym = Event::KEY_W;
                                                break;
                                            case 'a':
                                                tmp.key.sym = Event::KEY_A;
                                                break;
                                            case 's':
                                                tmp.key.sym = Event::KEY_S;
                                                break;
                                            case 'd':
                                                tmp.key.sym = Event::KEY_D;
                                                break;

                                        }
                                    }
                                    break;
                                    case DATA_KEY_UP:
                                    {

                                    }
                                    break;
                                    case DATA_GAMEUPDATE:
                                    {
                                        tmp.type = Event::EVENT_GAMEUPDATE;
                                        tmp.id = net->findId(std::string(host), std::string(serv));
                                        tmp.update.x = frame.body.data.x;
                                        tmp.update.y = frame.body.data.y;
                                        tmp.update.character = frame.body.data.key;
                                        //log(LG_DEBUG, "Network::run got game update x:%d y:%d char:%c", tmp.update.x, tmp.update.y, tmp.update.character);

                                    }
                                    break;
                                }

                                net->addEvent(tmp);//.pushBack(tmp);

                            }
                            break;
                        break;
                        case ACK:
                            //log(LG_DEBUG, "THREAD::Network::run Got ack frame");
                        break;
                        default:
                            log(LG_ERROR, "THREAD::Network::run Couldn't find frame type :(");
                            break;

                    }

                }

            }
            else if(nfds == 0)
            {
                log(LG_DEBUG, "THREAD::Network::run No events");
            }
            else if(nfds == -1)
            {
                log(LG_ERROR, "THREAD::Network::run epoll_wait returned error: %s", strerror(errno));
            }

        }
    }
}

bool Network::sendACK(int fd, int ack_num, struct sockaddr_storage peer_addr, socklen_t peer_addr_len)
{
    bool retVal = false;

    NET_Frame frame;
    frame.type = ACK;
    frame.num = ack_num;

    SendFrame(fd, frame, peer_addr, peer_addr_len);

    return retVal;
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

    log(LG_DEBUG, "Created new connection from %s:%s", host, serv);
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

    eventList = newEvent;//.pushBack(newEvent);

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
    if(pthread_mutex_lock(&mutex) == 0)
    {
        tmp = eventList;//.popFront();
        eventList = Event();
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
    NET_Frame frame;
    Event::Net_Event net = event.net;

    switch(net.net_type)
    {
    case Event::EVENT_NET_CONNECT:
        {
            log(LG_DEBUG, "Network::sendEvent Event::CONNECT, sending connection request to %s:%s", net.address.c_str(), net.port.c_str());
            if(!setup)
            {
                event.id = nextId;
                nextId++;
            }

            Setup(net.address, net.port, MODE_CLIENT, event.id);
            connection tmp;
            tmp = connections[event.id];
            tmp.id = event.id;
            tmp.address = net.address;
            tmp.port = net.port;
            tmp.status = Event::EVENT_NET_CONNECT;
            connections[event.id] = tmp;
            frame.type = CONNECTION_REQUEST;
        }
        break;
    case Event::EVENT_NET_CONNECT_ACK:
        {
            log(LG_DEBUG, "Network::sendEvent Event::EVENT_NET_CONNECT_ACK event received");
            frame.type = CONNECTION_REQUEST_ACK;
        }
        break;
    default:
        log(LG_ERROR, "ERROR:Network::sendEvent unknown event type");
    }

    connection to = connections[event.id];

    SendFrame(sfd, frame, to.peer_addr, to.peer_addr_len);
}

void Network::handleKeyEvent(Event event)
{
    NET_Frame frame;
    frame.type = DATA;
    frame.num = 0;
    Event::Key_Event key = event.key;

    if(key.key_type == Event::EVENT_KEY_DOWN)
    {
        frame.body.data.data_type = DATA_KEY_DOWN;
        frame.body.data.key = key.sym;
    }
    else if(key.key_type == Event::EVENT_KEY_UP)
    {
        frame.body.data.data_type = DATA_KEY_UP;
        frame.body.data.key = key.sym;
    }

    connection to = connections[event.id];

    SendFrame(sfd, frame, to.peer_addr, to.peer_addr_len);
}

void Network::handleGameUpdateEvent(Event event)
{
    NET_Frame frame;
    frame.type = DATA;
    frame.num = 0;

    frame.body.data.data_type = DATA_GAMEUPDATE;
    frame.body.data.x = event.update.x;
    frame.body.data.y = event.update.y;
    frame.body.data.key = event.update.character;

    connection to = connections[event.id];

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((sockaddr*)&to.peer_addr, to.peer_addr_len, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV);
    //log(LG_DEBUG, "Network::handleGameUpdate Sending x:%d y%d to %s:%s", frame.body.data.x, frame.body.data.y, host, serv);

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
        eventList = event;
        pthread_mutex_unlock(&mutex);
    }

}
