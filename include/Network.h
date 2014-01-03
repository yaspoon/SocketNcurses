#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <map>
#include "LinkedList.h"
#include "uuid/uuid.h"
#include "Event.h"
#include "Player.h"

#include <pthread.h>


enum DATA_type
{
    DATA_KEY_DOWN = 0,
    DATA_KEY_UP,
    DATA_GAMEUPDATE
};

typedef struct
{
    DATA_type data_type;
    char key;
    int x;
    int y;
} Data;

enum NET_Type
{
    ACK = 0,
    CONNECTION_REQUEST,
    CONNECTION_REQUEST_ACK,
    CONNECTION_ACCEPTED,
    CONNECTION_ACCEPTED_ACK,
    DATA

};

union NET_Frame_Body
{
    Data data;
    //time_t ping;
};

struct NET_Frame
{
    int num;
    NET_Type type;
    NET_Frame_Body body;
};

enum NET_Mode
{
    MODE_SERVER = 0,
    MODE_CLIENT
};

typedef struct
{
    Event::Event_Net_type status;
    unsigned int frame_num;
    std::string address;
    std::string port;
    sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    int id;
}connection;

class Network
{
    public:
        Network();
        Network(std::string address, std::string port, NET_Mode mode);
        virtual ~Network();
        void sendEvent(Event event);
        Event getEvent(void);
        Event::Event_Net_type getStatus();
        void setStatus(Event::Event_Net_type newStatus);

        bool Setup(std::string address, std::string port, NET_Mode mode, int id = 0);
    protected:
    private:
        int sfd;
        int nextId;
        std::map<int, connection> connections;
        //LinkedList<Event> eventList;
        Event eventList;
        Event::Event_Net_type status;
        bool setup;

        pthread_t runThread;
        pthread_mutex_t mutex;
        LinkedList<struct NET_Frame> frameQueue;

        bool SendFrame(int fd, NET_Frame frame, sockaddr_storage peer, socklen_t peer_len);
        bool sendACK(int fd, int ack_num, struct sockaddr_storage peer_addr, socklen_t peer_addr_len);
        int ReceiveFrame(int fd, NET_Frame *frame, sockaddr_storage *peer, socklen_t *peer_len);
        bool CreateSocket(std::string address, std::string port, NET_Mode sockType, int id = 0);
        static void* run(void *argument);
        void handleConnectionRequest( int frameNum, sockaddr_storage peer_addr, socklen_t peer_addr_len);
        size_t sizeofFrame(NET_Frame frame);
        void handleNetEvent(Event event);
        void handleKeyEvent(Event event);
        void handleGameUpdateEvent(Event event);
        int findId(std::string address, std::string port);
        void addEvent(Event event);
};

#endif // NETWORK_H
