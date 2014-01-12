#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <uuid/uuid.h>
#include "Player.h"
#include "Event.h"
#include <vector>


class Event
{
    public:
        enum Event_Net_type
        {
            EVENT_NET_UNKNOWN = 0,
            EVENT_NET_CONNECT,
            EVENT_NET_CONNECT_ACK,
            EVENT_NET_CONNECTED,
            EVENT_NET_CONNECTED_ACK,
            EVENT_NET_CONNECTING,
            EVENT_NET_DISCONNECT,
            EVENT_NET_DISCONNECTED
        };

        enum Event_type
        {
            EVENT_UNKNOWN = 0,
            EVENT_NET,
            EVENT_KEY,
            EVENT_GAMEUPDATE
        };

        enum Event_Key_type
        {
            EVENT_KEY_DOWN = 0,
            EVENT_KEY_UP
        };

        typedef struct
        {
            Event_Net_type net_type;
            std::string address;
            std::string port;
        } Net_Event;

        enum Key_Sym
        {
            KEY_A = 'a',
            KEY_W = 'w',
            KEY_S = 's',
            KEY_D = 'd'
        };

        typedef struct
        {
            Event_Key_type key_type;
            Key_Sym sym;

        } Key_Event;

        typedef struct
        {
            std::vector<Ent> entities;
        } GameUpdate_Event;

        Event_type type;
        Net_Event net;
        Key_Event key;
        GameUpdate_Event update;
        int id;

        Event();
        //void operator=(Event event);
        virtual ~Event();
    protected:
    private:
};

#endif // EVENT_H
