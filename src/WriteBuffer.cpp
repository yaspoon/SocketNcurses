#include "WriteBuffer.h"

WriteBuffer::WriteBuffer()
:Buffer()
{
}

WriteBuffer::WriteBuffer(Buffer *copy)
:Buffer(*copy)
{

}

WriteBuffer::~WriteBuffer()
{
}

bool WriteBuffer::SerialiseInt(int64_t input, uint32_t nobits)
{
	return Buffer::write(input, nobits) == OKAY ? true : false; 
}

bool WriteBuffer::SerialiseUint(uint64_t input, uint32_t nobits)
{
	return Buffer::write(input, nobits) == OKAY ? true : false;
}

bool WriteBuffer::SerialiseBits(uint64_t input, uint32_t nobits)
{
	return Buffer::write(input, nobits) == OKAY ? true : false;
}
