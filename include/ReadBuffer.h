#ifndef READ_BUFFER_H
#define READ_BUFFER_H

#include "Buffer.h"

class ReadBuffer: public Buffer
{
private:
public:
	static const bool isWriting = false;
	static const bool isReading = true;
	ReadBuffer();
	ReadBuffer(Buffer &input);
	ReadBuffer(Buffer *copy);
	~ReadBuffer();
	bool SerialiseInt(int64_t &input, uint32_t nobits);
	bool SerialiseUint(uint64_t &input, uint32_t nobits);
	bool SerialiseBits(uint64_t &input, uint32_t nobits);
};
#endif
