#ifndef NETWORK_STATISTICS_H
#define NETWORK_STATISTICS_H

#include <pthread.h>

enum stat_mode
{
    STAT_UNKNOWN=0,
    STAT_IN,
    STAT_OUT
};


class NetworkStatistics
{
    public:
        NetworkStatistics();
        virtual ~NetworkStatistics();
        float getMebibytes(stat_mode mode);
        void addBytes(int bytes, stat_mode mode);
    protected:
    private:
        int outBytes;
        int outKibibytes;
        int outMebibytes;
        int inBytes;
        int inKibibytes;
        int inMebibytes;

        pthread_mutex_t lock;
        int getBytes(stat_mode mode);
        float getKibibytes(stat_mode mode);
        void addKibibytes(int kibibytes, stat_mode mode);
        void addMebibytes(int mebibytes, stat_mode mode);
};

#endif // NETWORK_STATISTICS_H
