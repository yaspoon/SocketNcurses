#include "Frame.h"
#include <iostream>

#include "Stream.h"
#include "ReadBuffer.h"
#include "WriteBuffer.h"

Frame::Frame()
{
	type = ERROR;
	num = 0;
}

Frame::Frame(uint8_t newType, uint32_t newNum)
{
	type = newType;
	num = newNum;
}

Frame::~Frame()
{
}

uint8_t Frame::getType()
{
	return type;
}

uint32_t Frame::getNum()
{
	return num;
}

uint32_t Frame::getId()
{
	return id;
}

void Frame::setType(uint8_t newType)
{
	type = newType;
}

void Frame::setNum(uint32_t newNum)
{
	num = newNum;
}

void Frame::setId(uint32_t newId)
{
	id = newId;
}

template <typename Stream> bool Frame::serialise(Stream &stream)
{
	if(stream.isWriting)
	{
		std::cout << "Writer" << std::endl;
	}

	serialise_uint(stream, type, Frame::TYPE_START, Frame::TYPE_END);
	serialise_uint(stream, id, 0, UINT32_MAX);
	serialise_uint(stream, num, 0, UINT32_MAX);

	if(stream.isReading)
	{
		std::cout << "Reader" << std::endl;
	}

	return true; /*This is gross, the serialise_* macros have return statements in them. So returning here is par for the course...*/
}

/*Explicitly instanciate expected template parameters for serialise*/
template bool Frame::serialise<ReadBuffer>(ReadBuffer&);
template bool Frame::serialise<WriteBuffer>(WriteBuffer&);
