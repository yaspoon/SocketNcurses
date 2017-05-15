#include "Event.h"
#include "Common.h"

Event::Event()
{
    type = EVENT_DEFAULT;
    id = -1;
}

Event::Event(Event_type newType, int newId)
{
	type = newType;
	id = newId;
}

Event::~Event()
{
    //dtor
}

/*void Event::operator=(Event event)
{
    //log(LG_DEBUG, "Event::operator=");
    this->type = event.type;
    this->id = event.id;

    switch(event.type)
    {
        case EVENT_NET:
        {
            this->net = event.net;
        }
        break;
        case EVENT_KEY:
        {
            this->key = event.key;
        }
        break;
        case EVENT_GAMEUPDATE:
        {
            this->update.character = event.update.character;
            this->update.x = event.update.x;
            this->update.y = event.update.y;
        }
        break;
    }
}*/

void Event::setStream(const Buffer input)
{
	stream = input;
}

void Event::setId(int newId)
{
	id = newId;
}
