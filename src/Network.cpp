#include "Network.h"
#include "Common.h"
#include <string.h>

Network::Network()
{
   frame_num = 0;
   sfd = -1;
   uuid_generate(this->id);
}

Network::~Network()
{
    //dtor
}

bool Network::ConnectTo(std::string address, std::string port, SOCK_Type SockType)
{
    int retVal = false;
    sockaddr_storage peer;
    socklen_t peer_len;
    int tmpSfd = CreateSocket(address, port, SockType, &peer, &peer_len);

    if(tmpSfd > 0)
    {
        sfd = tmpSfd;
        log(LG_DEBUG, "Created socket to server, asking for authentication now");

        SendFrame(CONNECTION_REQUEST, peer, peer_len);

        NET_Type recType;
        sockaddr_storage recPeer;
        socklen_t recPeer_len;

        int rec = ReceiveFrame(&recType, &recPeer, &recPeer_len);

        if(recType == CONNECTION_ACCEPTED)
        {
            retVal = true;
            log(LG_DEBUG, "Connection Accepted by host");
        }


    }
    else
    {
        log(LG_ERROR, "Failed to connect to server %s port %s", address.c_str(), port.c_str());
    }

    return retVal;
}

bool Network::SendFrame(NET_Type type, sockaddr_storage peer, socklen_t peer_len)
{
    bool retVal = false;

    NET_Frame frame;

    frame.type = type;
    frame.num = frame_num;
    uuid_copy(frame.id, this->id);

    if(sendto(sfd, &frame, sizeof(NET_Frame), 0, (struct sockaddr*) &peer, peer_len) == sizeof(NET_Frame))
    {
        log(LG_DEBUG, "Network::SendFrame Wrote frame successfully, waiting for ack");

    }
    else
    {
        log(LG_ERROR, "Network::SendFrame Failed to write frame");
    }



    return retVal;
}

int Network::ReceiveFrame(NET_Type *type, struct sockaddr_storage *peer, socklen_t *peer_len)
{
    NET_Frame frame;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(struct sockaddr_storage);

    int nread = recvfrom(sfd, &frame, sizeof(frame), 0, (struct sockaddr*)&peer_addr, &peer_addr_len);

    if(nread != -1)
    {
        log(LG_DEBUG, "Network::ReceiveFrame Read a frame from socket");
        (*type) = frame.type;
        memcpy(peer, &peer_addr, sizeof(peer_addr));
        memcpy(peer_len, &peer_addr_len, sizeof(peer_addr_len));

        if(ack.type == ACK)
        {
            if(ack.num == frame_num)
            {
                frame_num += 1;
                retVal = true;
            }
            else
            {
                log(LG_ERROR, "Network::SendFrame Received ACK but frame number is wrong, expected %d, got %d", frame_num, ack.num);
            }
        }
        else
        {
            log(LG_ERROR, "Network::SendFrame Recevied frame, but it's not a ACK");
        }
    }
    else
    {
        log(LG_ERROR, "Network::ReceiveFrame Failed to read from socket");
    }

    return nread;

}

int Network::CreateSocket(std::string address, std::string port, SOCK_Type sockType, struct sockaddr_storage *peer, socklen_t *peer_len)
{
    int ret;
    int retVal = -1;

    struct addrinfo hints;
    struct addrinfo *info;

    if(sockType == SOCK_CLIENT)
    {
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = 0;
        hints.ai_protocol = 0;
    }
    else if(sockType == SOCK_SERVER)
    {
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_protocol = 0;
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;
    }

    log(LG_DEBUG, "Attempting to create socket to address %s, port %s", address.c_str(), port.c_str());

    if(sockType == SOCK_CLIENT)
    {
        ret = getaddrinfo(address.c_str(), port.c_str(), &hints, &info);
    }
    else
    {
        ret = getaddrinfo(NULL, port.c_str(), &hints, &info);
    }


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
                if(sockType == SOCK_CLIENT)
                {
                    if(connect(retVal, rp->ai_addr, rp->ai_addrlen) != -1)
                    {
                        char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
                        if(getnameinfo(rp->ai_addr,rp->ai_addrlen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
                        {
                            log(LG_DEBUG, "Network::CreateSocket Client connected socket to host %s with service %s", hbuf, sbuf);
                        }
                        else
                        {
                            log(LG_DEBUG, "Network::CreateSocket Client connected socket to UNKNOWN");
                        }
                        memcpy(peer, rp->ai_addr, sizeof(rp->ai_addr));
                        memcpy(peer_len, &(rp->ai_addrlen), sizeof(rp->ai_addrlen));
                        success = true;
                        this->sfd = retVal;
                        break; //So dirty D:
                    }
                }
                else if(sockType == SOCK_SERVER)
                {
                    if(bind(retVal, rp->ai_addr, rp->ai_addrlen) == 0)
                    {
                        log(LG_DEBUG, "Network::CreateSocket Bound socket for listening as server");
                        memcpy(peer, rp->ai_addr, sizeof(rp->ai_addr));
                        memcpy(peer_len, &(rp->ai_addrlen), sizeof(rp->ai_addrlen));
                        success = true;
                        this->sfd = retVal; //So dirty fix later...
                        break;
                    }
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
