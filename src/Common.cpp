#include "Common.h"
#include <sstream>
#include <string>
#include <string.h> //Seems so odd having both... :/

#include "Timer.h"

typedef enum
{
    NONE = 0,
    INT,
    DOUBLE,
    CHAR,
    STRING
} formatType_t;

struct mark_s
{
    formatType_t type;
    int offset;
};

void doLog(FILE* dest, std::string src);
formatType_t markType(char mark);
int formatCount(char *format);
void markTypes(struct mark_s ** marks, char *format);

void log(loglevel level, char *format, ...)
{
    va_list arguments;
    FILE* output = stdout;
    std::string apndStr = "";

    switch(level)
    {
        case LG_STANDARD:
            output = stdout;
            break;
        case LG_DEBUG:
            if(debug)
            {
                output = stdout;
                apndStr = "DEBUG:";
            }
            else
            {
                return; //So dirty I know but more efficient then having to compile the output string only to not print it because your not in debug mode...
            }
            break;
        case LG_ERROR:
            output = stderr;
            apndStr = "ERROR:";
            break;
        default:
            break;
    }

    int markCount = formatCount(format);

    if(markCount == 0) //There is no formatting marks to change so just print that shit out :D
    {
        doLog(output, format);
    }
    else //There is formating marks. Better format some shit
    {
        va_start(arguments, format);

        std::string fmtCpy(format);

        struct mark_s *markList;
        markTypes(&markList, format);

        std::string outputString;
        int startOffset = 0;

        for(int i = 0; i < markCount; i++)
        {

            std::string strPart = fmtCpy.substr(startOffset, markList[i].offset);
            startOffset += markList[i].offset + 2;


            outputString += strPart;


            switch(markList[i].type)
            {
                case STRING:
                    {
                        char *tmp = va_arg(arguments, char*);
                        outputString += tmp;
                        break;
                    }
                case CHAR:
                    {
                        int tmpChar = va_arg(arguments, int);
                        outputString += (char)tmpChar;
                        break;
                    }
                case DOUBLE:
                    {
                        double tmpDbl = va_arg(arguments, double);
                        outputString += static_cast<std::ostringstream*>( &(std::ostringstream() << tmpDbl) )->str();
                        break;
                    }
                case INT:
                    {
                        int tmpInt = va_arg(arguments, int);
                        outputString += static_cast<std::ostringstream*>( &(std::ostringstream() << tmpInt) )->str();
                        break;
                    }
            }
        }

        if(startOffset <= strlen(format))
        {
            outputString += fmtCpy.substr(startOffset, fmtCpy.length() - startOffset);
        }

        doLog(output, outputString.c_str());

        delete[] markList;
        va_end(arguments);
    }
}

void doLog(FILE* dest, std::string src)
{
    fprintf(dest, "%s\n", src.c_str());
}

/*
*NAME:formatCount
*DESC:Helper function, when given a string formatted for printf
*it will return the number of formatting "marks" this is then
*useful to count the number of variable arguments.
*INPUT: character array
*OUTPUT: number of formatting marks
*MODIFIES: Nothing
*/
int formatCount(char *format)
{
    int strLen = strlen(format);
    int count = 0;

    for(int i = 0; i < strLen; i++)
    {
        if(format[i] == '%') //We might have a format mark
        {
            if( (markType(format[i+1]) != NONE))
            {
                count += 1;
            }
        }
    }

    return count;
}

formatType_t markType(char mark)
{
    formatType_t type = NONE;

    switch(mark)
    {
    case 's':
        type = STRING;
        break;
    case 'c':
        type = CHAR;
        break;
    case 'd':
        type = INT;
        break;
    case 'f':
        type = DOUBLE;
        break;
    default:
        type = NONE;
        break;
    }

    return type;
}

void markTypes(struct mark_s ** marks, char *format)
{
    int count = formatCount(format);
    int markCount = 0;
    int curPos = 0;
    int oldPos = -2;
    int len = strlen(format);

    (*marks) = new struct mark_s[count];

    int i = 0;

    while(curPos <= len && markCount <= count)
    {
        if(format[curPos] == '%')
        {
            formatType_t type = markType(format[curPos+1]);
            if( type != NONE)
            {
                (*marks)[i].type = type;
                (*marks)[i].offset = curPos - (oldPos + 2);
                oldPos = curPos;
                markCount++;
                i++;
            }
        }

        curPos++;
    }

}

void delay(int milliseconds)
{
    Timer delayTimer;
    delayTimer.start();

    while(delayTimer.getTime() < milliseconds)
    {

    }
}
