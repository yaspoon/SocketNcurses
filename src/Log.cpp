#include "Log.h"

namespace Log
{

Log::Log()
{
}

Log::Log(logType type)
{
	this->type = type;
}

Log::~Log()
{
}

Log Log::operator[](logType type)
{
	switch(type)
	{
		case DBG:
		std::cout << "DEBUG:";
		case STB:
		std::cout << "STUB:";
		break;
	}
	return Log(type);
}

Log &Log::operator<<(std::string input)
{
	std::cout << input;
	return *this;
}

Log &Log::operator<<(int input)
{
	std::cout << input;
	return *this;
}
}
