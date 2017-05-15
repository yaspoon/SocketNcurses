#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <uuid/uuid.h>
#include "Player.h"
#include "Event.h"
#include <vector>
#include "Stream.h"
#include "Buffer.h"


class Event
{
    public:
        enum Event_Net_type
        {
		EVENT_NET_START= 0,
		EVENT_NET_UNKNOWN,
		EVENT_NET_CONNECT,
		EVENT_NET_CONNECT_ACK,
		EVENT_NET_CONNECTED,
		EVENT_NET_CONNECTED_ACK,
		EVENT_NET_CONNECTING,
		EVENT_NET_DISCONNECT,
		EVENT_NET_DISCONNECTED,
		EVENT_NET_DATA,
		EVENT_NET_END
        };

        enum Event_type: uint32_t
        {
		EVENT_TYPE_START = 0,
		EVENT_DEFAULT,
		EVENT_NET,
		EVENT_KEY,
		EVENT_GAMEUPDATE,
		EVENT_TYPE_END
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

        Event_type type;
        Net_Event net;
        Key_Event key;
        int id;
	Buffer stream;

        Event();
	Event(Event_type newType, int newId);
        //void operator=(Event event);
        virtual ~Event();
	void setStream(const Buffer input);
	void setId(int newId);
    protected:
    private:
};

#endif // EVENT_H
