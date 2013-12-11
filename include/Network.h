#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

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
};

typedef enum SOCK_Type
{
    SOCK_SERVER = 0,
    SOCK_CLIENT
};

class Network
{
    public:
        Network();
        virtual ~Network();

        bool SendFrame(int fd, NET_Frame frame, sockaddr_storage peer, socklen_t peer_len);
        int ReceiveFrame(int fd, NET_Frame *frame, sockaddr_storage *peer, socklen_t *peer_len);
        int CreateSocket(std::string address, std::string port, SOCK_Type sockType, struct sockaddr_storage *peer, socklen_t *peer_len );
    protected:
    private:
        int frame_num;
        uuid_t id;
};

#endif // NETWORK_H
