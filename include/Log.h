#ifndef LOG_H
#define LOG_H

#include <iostream>

namespace Log
{

	typedef enum
	{
	    STD = 0,
	    DBG,
	    ERR,
	    STB,
	} logType;

	const std::string endl = "\n";

	class Log
	{
	private:
		logType type;
	public:

		Log();
		Log(logType type);
		~Log();
		Log operator[](logType level);
		Log &operator<<(std::string input);
		Log &operator<<(int input);
	};

}

extern Log::Log log;

#define DEFAULT_DEBUG_LINE __BASE_FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " "
#define DEBUG log[Log::DBG] << DEFAULT_DEBUG_LINE
#define DEBUG_END Log::endl;
#define STUB log[Log::STB] << DEFAULT_DEBUG_LINE
#define STUB_END Log::endl;
#endif
