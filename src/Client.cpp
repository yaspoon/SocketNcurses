#include "Client.h"

#include "Event.h"
#include "Timer.h"

#include <string.h>
#include <ncurses.h>




Client::Client()
{
    Server_Address = "127.0.0.1";
    Server_Port = "34567";
}

Client::Client(std::string address, std::string port)
{
    Server_Address = address;
    Server_Port = port;
}

Client::Client(std::string port)
{
    Server_Address = "127.0.0.1";
    Server_Port = port;
}

Client::~Client()
{
}

bool Client::connect()
{
    bool retVal = false;
    int connectCount = 0;
    Timer timer;

    Event tmp;
    tmp.type = Event::EVENT_NET;
    tmp.net.net_type = Event::EVENT_NET_CONNECT;
    tmp.net.address = Server_Address;
    tmp.net.port = Server_Port;


    net.sendEvent(tmp);
    timer.start();

    while(connectCount < CONNECTION_ATTEMPTS && !retVal)
    {

        if(net.getStatus() != Event::EVENT_NET_CONNECTED)
        {
            if(timer.getTime() > CONNECTION_TIMEOUT)
            {
                //log(LG_DEBUG, "Failed to receive connection after 2 seconds attempting again");
                net.sendEvent(tmp);
                connectCount++;
                timer.stop();
                timer.start();

            }
            else
            {
                Event tmp = net.getEvent();
                switch(tmp.type)
                {
                case Event::EVENT_NET:

                    if(tmp.net.net_type == Event::EVENT_NET_CONNECT_ACK)
                    {
                        Server_ID = tmp.id;
                        retVal = true;
                        log(LG_DEBUG, const_cast<char *>("Client::run got EVENT_CONNECT_ACK"));
                    }
                break;
                }
            }
        }
    }

    return retVal;
}

void Client::run()
{
    bool quit = false;
    char data;

    log(LG_DEBUG, const_cast<char *>("Connecting to %s:%s"), Server_Address.c_str(), Server_Port.c_str());
    if(connect())
    {
        initscr();
        cbreak();
        nodelay(stdscr, true);
        noecho();
        keypad(stdscr, TRUE);

        Timer timer;

        while(!quit)
        {
            timer.stop();
            timer.start();

            Event event = net.getEvent();
            handleEvent(event);
            data = getch();
            if(data != ERR)
            {
                switch(data)
                {
                    case 'w':
                        sendKey(Event::EVENT_KEY_DOWN, Event::KEY_W);
                        break;
                    case 'a':
                        sendKey(Event::EVENT_KEY_DOWN, Event::KEY_A);
                        break;
                    case 's':
                        sendKey(Event::EVENT_KEY_DOWN, Event::KEY_S);
                        break;
                    case 'd':
                        sendKey(Event::EVENT_KEY_DOWN, Event::KEY_D);
                        break;
                }
            }
            clear();
            mvprintw(player.y, player.x, "%c", player.character);

            refresh();

            if(data == 'q')
            {
                quit = true;
            }

            long int time = timer.getTime();
            if(time < (1000/MAXFPS))
            {
                usleep(((1000/60) - time) * 1000);
            }
        }

        endwin();
    }
    else
    {
        log(LG_ERROR, const_cast<char *>("Failed to connect to %s:%s after %d attemps"), Server_Address.c_str(), Server_Port.c_str(), CONNECTION_ATTEMPTS);
    }
}

void Client::sendKey(Event::Event_Key_type type, Event::Key_Sym sym)
{
    Event key;
    key.id = Server_ID;
    key.type = Event::EVENT_KEY;
    key.key.key_type = type;
    key.key.sym = sym;
    net.sendEvent(key);
}

void Client::handleEvent(Event event)
{
    switch(event.type)
    {
        case Event::EVENT_GAMEUPDATE:
        {
            player.character = event.update.character;
            player.x = event.update.x;
            player.y = event.update.y;
        }
        break;
    }
}

bool Client::connectToServer()
{
    /*int sfd;
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
    //frame.num = frameNum;

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

    return retVal;*/
}
