#include <iostream>

//General Includes to do stuff
#include <string.h>

//Includes from this project
#include "Common.h"
#include "Server.h"
#include "Client.h"
#include "Log.h"

#define NET_ADDR_LEN 16

using namespace std;

typedef enum
{
    UNKNOWN = 0,
    SERVER,
    CLIENT
} type_t;

bool quit;
bool debug;

/*
*NAME:checkForDebug
*DESC:This is inefficient but I can't think of a better way.
*Preprocesses all of the input looking for the -debug flag
*hat way any other flags that cause debug output will actually output...
*It means double processing the input so O(2n) but that's still O(n) but
*I don't feel any better about it...
*INPUT: number of arguments in string array, array of arguments as char arrays
*OUTPUT: nothing
*MODIFIES: sets debug flag to true if command line switch "-debug" is found
*/
void  checkForDebug(int numArgs, char *argv[])
{
    for(int i = 0; i < numArgs; i++)
    {
        if(strcmp(argv[i], "-debug") == 0)
        {
            debug = true;
            //log(LG_DEBUG, const_cast<char *>("Debug enabled"));
	    DEBUG << "Debug enabled";
        }
    }
}

int main(int argc, char* argv[])
{

    quit = false;
    debug = false;
    type_t mode = UNKNOWN;
    string address = "127.0.0.1";
    string port = "34567";
    int retVal = OKAY;

    checkForDebug(argc, argv);

    for(int i = 0; i < argc; i++)
    {
        if( strcmp(argv[i], "-server") == 0 )
        {
            mode = SERVER;
            DEBUG << "Mode is now Server" << DEBUG_END;

        }
        else if( strcmp(argv[i], "-client") == 0 )
        {
            mode = CLIENT;
            DEBUG << "Mode is now Client" << DEBUG_END;
        }
        else if( strcmp(argv[i], "-ip") == 0 )
        {
            if( (i+1) <= argc)
            {
                if(strlen(argv[i+1]) <= (NET_ADDR_LEN - 1))
                {
                    address = argv[i+1];
                    DEBUG << "IP set " << address << DEBUG_END;
                }
                else
                {
                    DEBUG << "Malformed IP address or your trying to use a hostname try -host instead" << DEBUG_END;
                }
            }
            else
            {
                DEBUG << "ip flag given but no ip address given afterwards :(" << DEBUG_END;
            }
        }
        else if(strcmp(argv[i], "-port") == 0)
        {
            if( (i+1) <= argc)
            {
                port = (argv[i+1]);
                DEBUG << "Port set to " <<  port << DEBUG_END;
            }
            else
            {
                DEBUG << "-port flag specified but no port given afterwards :(" << DEBUG_END;
            }
        }

    }

    if(mode == UNKNOWN)
    {
        DEBUG << "No mode chosen exiting" << DEBUG_END;
        retVal = ERROR;
    }
    else
    {
            if(mode == SERVER)
            {
                Server myServer(address, port);
                myServer.run();
            }
            else if(mode == CLIENT)
            {
                Client myClient(address, port);
                myClient.run();
            }
            else
            {
                DEBUG << "Somehow got into main loop with a bad mode, quiting~!" << DEBUG_END;
                quit = true;
            }
    }


    return retVal;
}
