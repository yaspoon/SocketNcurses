#ifndef COMMON_H
#define COMMON_H

//Standard Includes used all over the place, so might aswell include them in one place
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//Socket stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <uuid/uuid.h>

//Global Variables
extern bool quit;
extern bool debug;

typedef enum
{
    OKAY = 0,
    ERROR
} returnCodes_t;

typedef enum
{
    LG_STANDARD = 0,
    LG_DEBUG,
    LG_ERROR
} loglevel;

//Functions
void log(loglevel level, char *format, ...);
void delay(int milliseconds);

#endif // COMMON_H
