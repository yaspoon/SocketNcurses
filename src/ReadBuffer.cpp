#include "ReadBuffer.h"

ReadBuffer::ReadBuffer()
:Buffer()
{
}

ReadBuffer::ReadBuffer(Buffer &input)
:Buffer(input)
{
}

ReadBuffer::ReadBuffer(Buffer *copy)
:Buffer(*copy)
{
}

ReadBuffer::~ReadBuffer()
{
}

bool ReadBuffer::SerialiseInt(int64_t &input, uint32_t nobits)
{
	input = Buffer::read(nobits);
	return Buffer::getStatus() == OKAY;
}

bool ReadBuffer::SerialiseUint(uint64_t &input, uint32_t nobits)
{
	input = Buffer::read(nobits);
	return Buffer::getStatus() == OKAY;
}

bool ReadBuffer::SerialiseBits(uint64_t &input, uint32_t nobits)
{
	input = Buffer::read(nobits);
	return Buffer::getStatus() == OKAY;
}
