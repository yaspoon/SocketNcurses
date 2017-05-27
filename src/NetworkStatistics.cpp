#include "NetworkStatistics.h"
#include "Common.h"
#include "Log.h"


NetworkStatistics::NetworkStatistics()
: inBytes(0), inKibibytes(0), inMebibytes(0), outBytes(0), outKibibytes(0), outMebibytes(0) //initialisation list
{
    pthread_mutex_init(&lock, NULL);
}

NetworkStatistics::~NetworkStatistics()
{
    //dtor
}

int NetworkStatistics::getBytes(stat_mode mode)
{
    int bytes;
    if(true)//pthread_mutex_lock(&lock) == 0)
    {
        switch(mode)
        {
            case STAT_UNKNOWN:
            break;
            case STAT_IN:
                bytes = inBytes + (inKibibytes * 1024) + (inMebibytes * 1024 * 1024);
            break;
            case STAT_OUT:
                bytes = outBytes + (outKibibytes * 1024) + (outMebibytes * 1024 * 1024);
            break;
            default:
            break;
        }
        //pthread_mutex_unlock(&lock);
    }
    else
    {
        DEBUG << "Failed to lock" << DEBUG_END;
    }

    return bytes;
}

float NetworkStatistics::getKibibytes(stat_mode mode)
{
    float kibibytes;

    if(true)//pthread_mutex_lock(&lock) == 0)
    {
        switch(mode)
        {
            case STAT_UNKNOWN:
            break;
            case STAT_IN:
                kibibytes = inKibibytes + (inBytes / 1024);
                kibibytes += inMebibytes * 1024;
            break;
            case STAT_OUT:
                kibibytes = outKibibytes + (outBytes / 1024);
                kibibytes += outMebibytes * 1024;
            break;
            default:
            break;
        }
        //pthread_mutex_unlock(&lock);
    }
    else
    {
        DEBUG << "Failed to lock" << DEBUG_END;
    }

    return kibibytes;
}

float NetworkStatistics::getMebibytes(stat_mode mode)
{
    float mebibytes = 0;

    if(pthread_mutex_trylock(&lock) == 0)
    {
        switch(mode)
        {
            case STAT_UNKNOWN:
            break;
            case STAT_IN:
            {
                mebibytes = inMebibytes;
                float tmpKibi = inKibibytes + (inBytes / 1024);
                mebibytes += tmpKibi / 1024;
            }
            break;
            case STAT_OUT:
            {
                mebibytes = outMebibytes;
                float tmpKibi = outKibibytes + (outBytes / 1024);
                mebibytes += tmpKibi / 1024;
            }
            break;
            default:
            break;
        }
        pthread_mutex_unlock(&lock);
    }
    else
    {
        DEBUG << "Failed to lock" << DEBUG_END;
    }

    return mebibytes;
}

void NetworkStatistics::addBytes(int bytes, stat_mode mode)
{
    if(pthread_mutex_lock(&lock) == 0)
    {
        switch(mode)
        {
            case STAT_UNKNOWN:
                DEBUG << "NetworkStatistics::addbytes,Mode is unknown so this does nothing.." << DEBUG_END;
            break;
            case STAT_IN:
            {
                int newBytes = inBytes + bytes;
                if(newBytes > 1023)
                {
                    int numKibi = newBytes / 1024;
                    addKibibytes(numKibi, mode);

                    int numBytes = newBytes - (numKibi * 1024);
                    if(numBytes > 0)
                    {
                        inBytes = numBytes;
                    }
                }
                else
                {
                    inBytes = newBytes;
                }
            }
            break;
            case STAT_OUT:
            {
                int newBytes = outBytes + bytes;
                if(newBytes > 1023)
                {
                    int numKibi = newBytes / 1024;
                    addKibibytes(numKibi, mode);

                    int numBytes = newBytes - (numKibi * 1024);
                    if(numBytes > 0)
                    {
                        outBytes = numBytes;
                    }
                }
                else
                {
                    outBytes = newBytes;
                }
            }
            break;
            default:
            break;
        }
        pthread_mutex_unlock(&lock);
    }
    else
    {
        DEBUG << "Failed to lock" << DEBUG_END;
    }
}

void NetworkStatistics::addKibibytes(int kibibytes, stat_mode mode)
{
    switch(mode)
    {
        case STAT_UNKNOWN:
            DEBUG << "NetworkStatistics::addkibibytes,Mode is unknown so this does nothing.." << DEBUG_END;
        break;
        case STAT_IN:
        {
            int newKibi = inKibibytes + kibibytes;
            if(newKibi > 1023)
            {
                int numMebi = newKibi / 1024;
                addMebibytes(numMebi, mode);

                int numKibi = newKibi - (numMebi * 1024);
                if(numKibi > 0)
                {
                    inKibibytes = numKibi;
                }
            }
            else
            {
                inKibibytes = newKibi;
            }
        }
        break;
        case STAT_OUT:
        {
            int newKibi = outKibibytes + kibibytes;
            if(newKibi > 1023)
            {
                int numMebi = newKibi / 1024;
                addMebibytes(numMebi, mode);

                int numKibi = newKibi - (numMebi * 1024);
                if(numKibi > 0)
                {
                    outKibibytes = numKibi;
                }
            }
            else
            {
                outKibibytes = newKibi;
            }
        }
        break;
        default:
        break;

    }
}

void NetworkStatistics::addMebibytes(int mebibytes, stat_mode mode)
{
    switch(mode)
    {
        case STAT_UNKNOWN:
            DEBUG << "NetworkStatistics::addMebibytes,Mode is unknown so this does nothing.." << DEBUG_END;
            break;
        case STAT_IN:
            inMebibytes += mebibytes;
            break;
        case STAT_OUT:
            outMebibytes += mebibytes;
            break;
        default:
            break;
    }
}
