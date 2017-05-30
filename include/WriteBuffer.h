#ifndef WRITE_BUFFER_H
#define WRITE_BUFFER_H
#include "Buffer.h"

class WriteBuffer: public Buffer
{
private:
public:
	static const bool isWriting = true;
	static const bool isReading = false;
	WriteBuffer();
	WriteBuffer(Buffer *copy);
	~WriteBuffer();
	bool ByteAlign();
	bool SerialiseInt(int64_t input, uint32_t nobits);
	bool SerialiseUint(uint64_t input, uint32_t nobits);
	bool SerialiseBits(uint64_t input, uint32_t nobits);
	bool SerialiseBuffer(Buffer input);
	bool SerialiseArray(char *array, size_t nobytes);
};
#endif
