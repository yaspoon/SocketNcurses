#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include "Common.h"

const static unsigned int BUFFER_DEFAULT_SIZE = 100;

typedef struct uint128
{
	uint64_t high;
	uint64_t low;
} uint128_t;

class Buffer
{
private:
	int len;
	int index;
	int read_index;
	int bitsWritten;
	uint64_t scratch;
	uint64_t *data;
	const static int bitsPerWord = sizeof(scratch) * 8;
	int scratchBits;
	returnCodes_t status;
public:
	Buffer();
	Buffer(const Buffer &copy);
	~Buffer();
	Buffer &operator=(const Buffer &copy);
	returnCodes_t write(uint64_t input, uint32_t nobits);
	uint64_t read(uint32_t nobits);
	void fillFromBuffer(uint64_t *buf, size_t nbytes);
	void printData();
	void flush();
	void reset();
	returnCodes_t getStatus();
	int totalBits();
	int bitsLeft();
	int totalBytes();
	int bytesLeft();

	uint64_t *bytes();

	/*Ugggghhhh, it stops lots of typecasting in the caller code atleast*/
	returnCodes_t writeBool(bool input);
	returnCodes_t writeInt8(int8_t input);
	returnCodes_t writeUint8(uint8_t input);
	returnCodes_t writeInt16(int16_t input);
	returnCodes_t writeUint16(uint16_t input);
	returnCodes_t writeEnum(uint32_t input); //C++ 11 has typed enums, but it's suppose to be int so I guess this is more compiler compatible?
	returnCodes_t writeInt32(int32_t input);
	returnCodes_t writeUint32(uint32_t input);
	returnCodes_t writeInt64(int64_t input);
	returnCodes_t writeUint64(uint64_t input);
	returnCodes_t writeBuffer(Buffer input);

	bool readBool();
	int8_t readInt8();
	uint8_t readUint8();
	int16_t readInt16();
	uint16_t readUint16();
	uint32_t readEnum();
	int32_t readInt32();
	uint32_t readUint32();
	int64_t readInt64();
	uint64_t readUint64();
};
#endif
