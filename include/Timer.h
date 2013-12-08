#ifndef TIMER_H
#define TIMER_H

#include <time.h>


class Timer
{
    public:
        Timer();
        virtual ~Timer();
        void start();
        void stop();
        void pause();
        void unpause();


        bool isPaused();
        bool isStarted();

        long int getTime();
        void printTime();

    protected:
    private:
        struct timespec startTime;
        struct timespec pauseTime;
        bool started;
        bool paused;
};

#endif // TIMER_H
