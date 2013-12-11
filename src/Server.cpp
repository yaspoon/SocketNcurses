#include "Server.h"

#include <string.h>
#include "Network.h"
#include <sys/epoll.h>

Server::Server()
{

    SV_Address = "127.0.0.1";
    SV_Port = "34567";
    uuid_generate(SV_ID);
}

Server::Server(std::string address, std::string port)
{
    SV_Address = address;
    SV_Port = port;
    uuid_generate(SV_ID);
}

Server::Server(std::string port)
{
    SV_Address = "127.0.0.1";
    SV_Port = port;
    uuid_generate(SV_ID);
}

Server::~Server()
{

}

returnCodes_t Server::run()
{
    int epfd = epoll_create(10);
    struct sockaddr_storage address;
    socklen_t address_len;
    int listener = SV_Net.CreateSocket(SV_Address, SV_Port, SOCK_SERVER, &address, &address_len);

    NET_Type type;
    sockaddr_storage client;
    socklen_t client_len;

    struct epoll_event ev;

    ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
    ev.data.fd = listener;
    int MAX_EVENTS = 64;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &ev) == 0)
    {
        log(LG_DEBUG, "Added socket fd to epoll fine");
        struct epoll_event events[MAX_EVENTS];

        //while(1)
        {

            //log(LG_DEBUG, "Polling for network activity");
            int nfds = epoll_wait(epfd, events, MAX_EVENTS, 10000);

            if(nfds > 0)
            {
                log(LG_DEBUG, "Events received after polling YAY");

                for(int i = 0; i < nfds; i++)
                {
                    NET_Frame frame;
                    struct sockaddr_storage peer_addr;
                    socklen_t peer_addr_len = sizeof(struct sockaddr_storage);

                    int nread = SV_Net.ReceiveFrame(events[i].data.fd, &frame, &peer_addr, &peer_addr_len );

                    char host[NI_MAXHOST];
                    char serv[NI_MAXSERV];

                    getnameinfo((struct sockaddr*)&peer_addr, peer_addr_len, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV);

                    log(LG_DEBUG, "Received %d bytes from %s:%s", nread, host, serv);

                    handleFrame(events[i].data.fd, frame, peer_addr, peer_addr_len);

                }

            }
            else if(nfds == 0)
            {
                log(LG_DEBUG, "No events after 10 second");
            }
            else if(nfds == -1)
            {
                log(LG_ERROR, "epoll_wait returned error: %s", strerror(errno));
            }

        }
    }
    else
    {
        log(LG_ERROR, "Failed to add fds to epoll events error:%s", strerror(errno));
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

bool Server::handleFrame(int fd, NET_Frame frame, struct sockaddr_storage peer_addr, socklen_t peer_addr_len)
{
    bool retVal = true;

    switch(frame.type)
    {
    case CONNECTION_REQUEST:
        {
            log(LG_DEBUG, "Server::run Connection request received");

            addClient(frame.id, peer_addr, peer_addr_len, frame.num);

            NET_Frame replyFrame;
            replyFrame.num = frame.num;
            replyFrame.type = ACK;

            SV_Net.SendFrame(fd, replyFrame, peer_addr, peer_addr_len);
        }
    }

    return retVal;
}

