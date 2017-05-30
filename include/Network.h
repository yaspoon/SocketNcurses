#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <list>
#include <map>
#include "LinkedList.h"
#include "uuid/uuid.h"
#include "Event.h"
#include "Players.h"
#include "Entitys.h"
#include "NetworkStatistics.h"
#include "Buffer.h"
#include "Frame.h"

#include <pthread.h>

const uint32_t MAX_NETWORK_BUFFER_WORDS = 188;

/*
struct NET_frame_data {
	uint8_t data[MAX_NETWORK_BUFFER_BYTES - sizeof(NET_Frame)];
};

struct NET_Frame {
	char magic[4] = "NUB" + VERSION;
	uint32_t crc;
	int num;
	uint8_t type;
	//Net_frame_data data;
	char end_magic[4] = "END" + VERSION
}
*/

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

        bool Setup(std::string address, std::string port, NET_Mode mode);
        NetworkStatistics getStatistics();
    protected:
    private:
	struct drand48_data drand;
	int id;
        int sfd;
        int nextId;
        std::map<int, connection> connections;
	std::list<Event> eventList;
        Event::Event_Net_type status;
        bool setup;

        pthread_t runThread;
        pthread_mutex_t mutex;
        NetworkStatistics statistics;

        bool SendFrame(int fd, Frame frame, sockaddr_storage peer, socklen_t peer_len);
        bool sendACK(int fd, int ack_num, struct sockaddr_storage peer_addr, socklen_t peer_addr_len);
        static int ReceiveFrame(int fd, Buffer &frame, struct sockaddr_storage *peer, socklen_t *peer_len);
        bool CreateSocket(std::string address, std::string port, NET_Mode sockType, int id = 0);
        static void* run(void *argument);
        void handleConnectionRequest( int frameNum, sockaddr_storage peer_addr, socklen_t peer_addr_len);
        void handleNetEvent(Event event);
        void handleKeyEvent(Event event);
        void handleGameUpdateEvent(Event event);
        int findId(std::string address, std::string port);
        void addEvent(Event event);
};

#endif // NETWORK_H
