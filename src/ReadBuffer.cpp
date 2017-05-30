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

bool ReadBuffer::SerialiseBuffer(Buffer &output)
{
	bool retval = true;
	while(bitsLeft() > bitsPerWord && retval)
	{
		retval = output.writeUint64(readUint64());
	}
	uint32_t bits = bitsLeft();
	if(retval == true)
	{
		output.write(read(bits), bits);
	}

	return retval;
}
