#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <map>
#include "LinkedList.h"

#include "Event.h"

#include <pthread.h>

typedef enum
{
    ACK = 0,
    CONNECTION_REQUEST,
    CONNECTION_ACCEPTED,
    DATA

} NET_Type;

struct NET_Frame
{
    int num;
    NET_Type type;
    uuid_t id;
    char data;
};

typedef enum NET_Mode
{
    MODE_SERVER = 0,
    MODE_CLIENT
};

typedef struct
{
    int frame_num;
    sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
}connection;

class Network
{
    public:
        Network();
        Network(std::string address, std::string port, NET_Mode mode);
        virtual ~Network();
        void sendData(char data);
        void sendEvent(Event event);
        Event getEvent(void);

        bool Setup();
    protected:
    private:
        bool listening;
        pthread_t runThread;
        std::string address;
        std::string port;
        NET_Mode mode;
        LinkedList<struct NET_Frame> frameQueue;

        static bool SendFrame(int fd, NET_Frame frame, sockaddr_storage peer, socklen_t peer_len);
        static bool sendACK(int fd, int ack_num, struct sockaddr_storage peer_addr, socklen_t peer_addr_len);
        static int ReceiveFrame(int fd, NET_Frame *frame, sockaddr_storage *peer, socklen_t *peer_len);
        int CreateSocket(std::string address, std::string port, NET_Mode sockType, struct sockaddr_storage *peer, socklen_t *peer_len );
        static void* run(void *arguments);
        static void handleConnectionRequest(uuid_t id, int frameNum, sockaddr_storage peer_addr, socklen_t peer_addr_len);
};

static int sfd;
static uuid_t id;
static std::map<std::string, connection> connections;
static connection client;
static LinkedList<Event> eventList;

#endif // NETWORK_H
