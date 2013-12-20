#include "Network.h"
#include "Common.h"
#include <string.h>

Network::Network()
{
   frame_num = 0;
   uuid_generate(this->id);
}

Network::~Network()
{
    //dtor
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

int Network::CreateSocket(std::string address, std::string port, SOCK_Type sockType, struct sockaddr_storage *peer, socklen_t *peer_len)
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
