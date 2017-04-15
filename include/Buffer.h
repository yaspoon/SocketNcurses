#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

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
	uint64_t scratch;
	typeof(scratch) *data;
	const static int bitsPerWord = sizeof(scratch) * 8;
	int scratchBits;
public:
	Buffer();
	~Buffer();
	void write(uint64_t input, uint32_t nobits);
	uint64_t read(uint32_t nobits);
	void flush();
	uint64_t getScratch();
	void printData();
	void reset();
};
#endif
