#include "Client.h"

#include "Log.h"

#include "Event.h"
#include "Timer.h"

#include <string.h>
#include <ncurses.h>
#include "ReadBuffer.h"
#include "WriteBuffer.h"


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
                        DEBUG << "Client::run got EVENT_CONNECT_ACK" << DEBUG_END;
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

    DEBUG << "Connecting to " << Server_Address << ":" << Server_Port << DEBUG_END;
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

            std::vector<Player> players = state.getPlayers();
            //mvprintw(1, 1, "Num players %d", players.size());

            for(int i = 0; i < players.size(); i++)
            {
                Player player = players[i];

                mvprintw(player.y, player.x, "%c", player.character);


            }

            printNetStats();

            refresh();

            if(data == 'q')
            {
                quit = true;
            }

            long int time = timer.getTime();
            if(time < (1000/MAXFPS))
            {
                usleep(((1000/MAXFPS) - time) * 1000);
            }
        }

        endwin();
    }
    else
    {
        DEBUG << "Failed to connect to " << Server_Address << ":" << Server_Port << "after " <<  CONNECTION_ATTEMPTS << "attemps" << DEBUG_END;
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
		ReadBuffer readBuffer(event.stream);
            state.serialise(readBuffer);
        }
        break;
    }
}

void Client::printNetStats()
{
    NetworkStatistics stats = net.getStatistics();

    float out = stats.getMebibytes(STAT_OUT);
    float in = stats.getMebibytes(STAT_IN);
    int maxX;
    int maxY;

    getmaxyx(stdscr, maxY, maxX);

    mvprintw(maxY - 2, 0, "OUT:%f MB's", out);
    mvprintw(maxY - 1, 0, "IN:%f MB's", in);



}
