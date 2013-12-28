#ifndef EVENT_H
#define EVENT_H


class Event
{
    public:
        typedef enum
        {
            CONNECT = 0,
            CONNECTED
        }Event_type;

        Event_type type;
        Event();
        virtual ~Event();
    protected:
    private:
};

#endif // EVENT_H
