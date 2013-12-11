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
    frameNum = 0;
}

Client::Client(std::string address, std::string port)
{
    Server_Address = address;
    Server_Port = port;
    uuid_generate(Client_ID);
    char id[37];
    uuid_unparse(Client_ID, id);
    log(LG_DEBUG, "Client ID:%s", id);
    frameNum = 0;
}

Client::Client(std::string port)
{
    Server_Address = "127.0.0.1";
    Server_Port = port;
    uuid_generate(Client_ID);
    char id[37];
    uuid_unparse(Client_ID, id);
    log(LG_DEBUG, "Client ID:%s", id);
    frameNum = 0;
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
        //log(LG_ERROR, "Failed to connect to server at %s port %s", Server_Address.c_str(), Server_Port.c_str());
    }
}

bool Client::connectToServer()
{
    int sfd;
    bool retVal = false;

    //CL_Net.ConnectTo(Server_Address, Server_Port, SOCK_CLIENT);
    struct sockaddr_storage server;
    socklen_t server_len = sizeof(server);
    //sfd = CL_Net.CreateSocket("", "0", SOCK_CLIENT, &server, &server_len);

    struct addrinfo hints;
    struct addrinfo *info;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    NET_Frame frame;
    log(LG_DEBUG, "Size of frame %d", sizeof(frame));
    frame.type = CONNECTION_REQUEST;
    frame.num = frameNum;

    log(LG_DEBUG, "Server:%s Port:%s", Server_Address.c_str(), Server_Port.c_str());

    int ret = getaddrinfo(Server_Address.c_str(), Server_Port.c_str(), &hints, &info);

    if(ret == 0)
    {

        log(LG_DEBUG, "Got addr info in client");
        for(struct addrinfo* rp = info; rp != NULL; rp = rp->ai_next)
        {
            sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            log(LG_DEBUG, "SFD=%d", sfd);

            int sent = sendto(sfd, &frame, sizeof(frame), 0, rp->ai_addr, rp->ai_addrlen);

            char host[NI_MAXHOST];
            char serv[NI_MAXSERV];

            getnameinfo(rp->ai_addr, rp->ai_addrlen, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV);

            if(sent != -1)
            {
                log(LG_DEBUG, "Sent %d bytes to %s:%s", sent, host, serv);
            }
            else
            {
                log(LG_ERROR, "Failed to send to server %s:%s reason:%s", host, serv, strerror(errno));
            }

            struct sockaddr_storage peer;
            socklen_t peer_len = sizeof(peer);
            NET_Frame inFrame;
            CL_Net.ReceiveFrame(sfd, &inFrame, &peer, &peer_len);

            if(inFrame.type == ACK)
            {
                if(inFrame.num == frameNum)
                {
                    log(LG_DEBUG, "Got ACK frame with right number");
                }
            }
        }
    }

    return retVal;
}
