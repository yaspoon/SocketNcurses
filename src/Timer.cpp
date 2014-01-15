#include "Timer.h"
#include "Common.h"
#include <string.h>

Timer::Timer()
{
    started = false;
    paused = false;
}

Timer::~Timer()
{
    //dtor
}

void Timer::start()
{
    if(!started)
    {
        started = true;
        clock_gettime(CLOCK_REALTIME, &startTime);
    }
    else
    {
        log(LG_DEBUG, const_cast<char *>("Timer already started at %d second %d nanoseconds"), startTime.tv_sec, startTime.tv_nsec);
    }
}

void Timer::stop()
{
    started = false;
    paused = false;
    memset(&startTime, 1, sizeof(struct timespec));
    memset(&pauseTime, 1, sizeof(struct timespec));
}

void Timer::pause()
{
    if(!paused)
    {
        paused = true;
        clock_gettime(CLOCK_REALTIME, &pauseTime);
        log(LG_DEBUG, const_cast<char *>("Paused timer at %d second %d nanoseconds"), pauseTime.tv_sec, pauseTime.tv_nsec);
    }
    else
    {
        log(LG_DEBUG, const_cast<char *>("Timer already Paused at %d second %d nanoseconds"), pauseTime.tv_sec, pauseTime.tv_nsec);
    }
}

void Timer::unpause()
{

}

bool Timer::isPaused()
{
    return paused;
}

bool Timer::isStarted()
{
    return started;
}

long int Timer::getTime()
{
    long int time = 0;

    if(started)
    {

        struct timespec tmp;

        clock_gettime(CLOCK_REALTIME, &tmp);

        tmp.tv_sec = tmp.tv_sec - startTime.tv_sec;
        tmp.tv_nsec = tmp.tv_nsec - startTime.tv_nsec;

        time = tmp.tv_sec * 1000; //Convert to milliseconds
        time += tmp.tv_nsec / (1000*1000);
    }

    return time;
}

void Timer::printTime()
{
    struct timespec tmp;
    clock_gettime(CLOCK_REALTIME, &tmp);
    log(LG_DEBUG, const_cast<char *>("Time %d seconds, %d nanoseconds"), tmp.tv_sec, tmp.tv_nsec );
}
