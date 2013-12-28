#include "Network.h"
#include "Common.h"
#include <string.h>

#include <sys/epoll.h>

Network::Network()
{
        listening = false;
        this->mode = MODE_CLIENT;
        uuid_generate(id);
}

Network::Network(std::string address, std::string port, NET_Mode mode)
{
    listening = false;
   this->address = address;
   this->port = port;
   this->mode = mode;
   uuid_generate(id);
}

Network::~Network()
{
    //dtor
}

bool Network::Setup()
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

    if(mode == MODE_SERVER)
    {
        ret = getaddrinfo(NULL, port.c_str(), &hints, &info);
    }
    else if(mode == MODE_CLIENT)
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
                if(mode == MODE_CLIENT)
                {
                    sfd = tmpSfd;
                    log(LG_DEBUG, "Network::Created socket successfully ready to send as client");
                    pthread_create(&runThread, NULL, run, NULL);
                    break;
                }
                else
                {
                    if(bind(tmpSfd, rp->ai_addr, rp->ai_addrlen) == 0)
                    {
                        sfd = tmpSfd;
                        log(LG_DEBUG, "Network::CreateSocket Bound socket for listening as server");
                        retVal = true;
                        pthread_create(&runThread, NULL, run, NULL);
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

bool Network::SendFrame(int fd, NET_Frame frame, sockaddr_storage peer, socklen_t peer_len)
{
    bool retVal = false;

    if(sendto(fd, &frame, sizeof(NET_Frame), 0, (struct sockaddr*) &peer, peer_len) == sizeof(NET_Frame))
    {
        log(LG_DEBUG, "Network::SendFrame Wrote frame successfully, waiting for ack");

    }
    else
    {
        log(LG_ERROR, "Network::SendFrame Failed to write frame");
    }



    return retVal;
}

int Network::ReceiveFrame(int fd, NET_Frame *frame, struct sockaddr_storage *peer, socklen_t *peer_len)
{
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(struct sockaddr_storage);

    int nread = recvfrom(fd, frame, sizeof(frame), 0, (struct sockaddr*)&peer_addr, &peer_addr_len);

    if(nread != -1)
    {
        log(LG_DEBUG, "Network::ReceiveFrame Read a frame from socket");
        memcpy(peer, &peer_addr, sizeof(peer_addr));
        memcpy(peer_len, &peer_addr_len, sizeof(peer_addr_len));

        /*if(frame.type == ACK)
        {
            if(frame.num == frame_num)
            {
                frame_num += 1;
            }
            else
            {
                log(LG_ERROR, "Network::SendFrame Received ACK but frame number is wrong, expected %d, got %d", frame_num, frame.num);
            }
        }
        else
        {
            SendFrame(fd, ACK, peer_addr, peer_addr_len);
            log(LG_ERROR, "Network::SendFrame Recevied frame, sending ACK");
        }*/
    }
    else
    {
        log(LG_ERROR, "Network::ReceiveFrame Failed to read from socket reason:%s", strerror(errno));
    }

    return nread;

}

int Network::CreateSocket(std::string address, std::string port, NET_Mode netType, struct sockaddr_storage *peer, socklen_t *peer_len)
{
    int ret;
    int retVal = -1;

    struct addrinfo hints;
    struct addrinfo *info;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    log(LG_DEBUG, "Attempting to create socket to address %s, port %s", address.c_str(), port.c_str());

    ret = getaddrinfo(NULL, port.c_str(), &hints, &info);

    if(ret == 0)
    {
        log(LG_DEBUG, "Got Address info, continuing");

        bool success = false;
        struct addrinfo* rp = NULL;

        for(rp = info; rp != NULL; rp = rp->ai_next)
        {
            retVal = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            log(LG_DEBUG, "Creating Socket");

            if(retVal != -1)
            {
                log(LG_DEBUG, "Created Socket successfully");
                if(bind(retVal, rp->ai_addr, rp->ai_addrlen) == 0)
                {
                    log(LG_DEBUG, "Network::CreateSocket Bound socket for listening as server");
                    memcpy(peer, rp->ai_addr, sizeof(rp->ai_addr));
                    memcpy(peer_len, &(rp->ai_addrlen), sizeof(rp->ai_addrlen));
                    success = true;
                    break;
                }
            }
        }

        freeaddrinfo(info);
    }
    else
    {
        log(LG_ERROR, "Client::CreateSocket Failed to get Address info Error:%s", gai_strerror(ret));
    }

    return retVal;
}

void* Network::run( void* arguments)
{
    log(LG_DEBUG, "THREAD::Network::run Added socket fd to epoll fine");
    struct epoll_event ev;
    struct epoll_event events[10];
    int epfd;

    ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
    ev.data.fd = sfd;
    epfd = epoll_create(10);

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev) == 0)
    {

        while(true)
        {

            log(LG_DEBUG, "THREAD::Network::run Polling for network activity");
            int nfds = epoll_wait(epfd, events, 10, -1);

            if(nfds > 0)
            {
                log(LG_DEBUG, "THREAD::Network::run Events received after polling YAY");

                for(int i = 0; i < nfds; i++)
                {
                    NET_Frame frame;
                    struct sockaddr_storage peer_addr;
                    socklen_t peer_addr_len = sizeof(struct sockaddr_storage);

                    int nread = recvfrom(events[i].data.fd, &frame, sizeof(frame), 0, (struct sockaddr*)&peer_addr, &peer_addr_len);

                    char host[NI_MAXHOST];
                    char serv[NI_MAXSERV];

                    getnameinfo((struct sockaddr*)&peer_addr, peer_addr_len, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV);

                    log(LG_DEBUG, "THREAD::Network::run Received %d bytes from %s:%s", nread, host, serv);

                    switch(frame.type)
                    {
                        case CONNECTION_REQUEST:
                            sendACK(sfd, frame.num, peer_addr, peer_addr_len);
                            handleConnectionRequest(frame.id, frame.num, peer_addr, peer_addr_len);
                            log(LG_DEBUG, "THREAD::Network::run Connnection request");
                        break;
                        case CONNECTION_ACCEPTED:
                            {
                                Event tmp;
                                tmp.type = Event::CONNECTED;
                                eventList.pushBack(tmp);
                                log(LG_DEBUG, "THREAD::Network::run Connection accepted");
                            }
                        break;
                        case DATA:

                            log(LG_DEBUG, "THREAD::Network::run Got data %c from %s:%s", frame.data, host, serv);
                            break;
                        break;
                        case ACK:
                            log(LG_DEBUG, "THREAD::Network::run Got ack frame");
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
    uuid_copy(frame.id, id);

    SendFrame(fd, frame, peer_addr, peer_addr_len);

    return retVal;
}

void Network::handleConnectionRequest(uuid_t id, int frameNum, sockaddr_storage peer_addr, socklen_t peer_addr_len)
 {
     char theId[37];
     uuid_parse(theId, id);
     std::string strId(theId);

        log(LG_DEBUG, "Created new connection");
        connection tmp;
        tmp.frame_num = frameNum;
        tmp.peer_addr = peer_addr;
        tmp.peer_addr_len = peer_addr_len;
        client = tmp;

        NET_Frame frame;
        frame.type = CONNECTION_ACCEPTED;
        frame.num = frameNum;
        uuid_copy(frame.id, id);

        SendFrame(sfd, frame, peer_addr, peer_addr_len);

 }

void Network::sendData(char data)
{
    log(LG_DEBUG, "Network::SendData");
    NET_Frame frame;
    frame.num = 0;
    frame.type = DATA;
    frame.data = data;
    uuid_copy(frame.id, id);

    struct addrinfo hints;
    struct addrinfo *info;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int ret = getaddrinfo(address.c_str(), port.c_str(), &hints, &info);

    if(ret == 0)
    {
        struct addrinfo *rp = info;
        for(rp = info; rp != NULL; rp = rp->ai_next)
        {
            log(LG_DEBUG, "Network::senddata: Sending %c framesize is %d", data, sizeof(frame));
            int sent = sendto(sfd, &frame, sizeof(frame), 0, rp->ai_addr, rp->ai_addrlen);
        }
    }
}

void Network::sendEvent(Event event)
{
    NET_Frame frame;
    frame.num = 0;
    uuid_copy(frame.id, id);

    switch(event.type)
    {
    case Event::CONNECT:
        log(LG_DEBUG, "Network::sendEvent Event::CONNECT, sending connection request");
        struct addrinfo hints;
        struct addrinfo *info;

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_protocol = IPPROTO_UDP;
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;
        getaddrinfo(address.c_str(), port.c_str(), &hints, &info);
        memcpy(&(client.peer_addr), info->ai_addr, sizeof(sockaddr));
        client.peer_addr_len = info->ai_addrlen;
        frame.type = CONNECTION_REQUEST;
        break;
    case Event::CONNECTED:
        frame.type = CONNECTION_ACCEPTED;
        break;
    }

    SendFrame(sfd, frame, client.peer_addr, client.peer_addr_len);
}

Event Network::getEvent(void)
{
    Event tmp = eventList.popFront();
    return tmp;
}
