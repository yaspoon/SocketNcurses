#include "Buffer.h"

#include <iostream>
#include <string.h>

Buffer::Buffer()
{
	len = BUFFER_DEFAULT_SIZE;
	index = 0;
	read_index = 0;
	data = new uint64_t[len];
	scratch = 0;
	scratchBits = 0;
}

Buffer::~Buffer()
{
	delete[] data;
}

void Buffer::write(uint64_t input, uint32_t nobits)
{
	uint64_t selectBits = 0;

	int newTotalBits = scratchBits + nobits;
	if(newTotalBits > bitsPerWord) //More bits then we can fit in a word
	{
		int freeBits = (bitsPerWord - scratchBits); //Number of bits left before this word is full
		int leftOver = nobits - freeBits; //Number of bits left over in the input
		scratch= scratch<< freeBits; //Copy the lower bits to the end of scratch.high
		scratch = scratch| (input >> leftOver); //Add overflow bits to scratch.high
		data[index] = scratch; //Flush word to buffer
		index += 1;
		scratch = 0; //Clear scratch high memory

		if(leftOver >= bitsPerWord) //Shifting by number of bits in a word or greater is undefined behaviour
		{
			selectBits = ~selectBits;
		}
		else
		{
			selectBits = ~((~selectBits) << leftOver);
		}
		scratch = scratch |  (selectBits & input);
		scratchBits = leftOver;
	}
	else
	{
		if(nobits >= bitsPerWord)
		{
			selectBits = ~selectBits;
		}
		else
		{
			selectBits = ~((~selectBits)<< nobits);
		}
		scratch = (scratch << nobits) | (selectBits & input);
		scratchBits += nobits;
	}
}

uint64_t Buffer::read(uint32_t nobits)
{
	typeof(scratch) output = 0;
	int32_t scratchDiff = scratchBits - nobits;
	typeof(scratch) bitSelector = ~0; //Set a word to all 1 bits
	if(scratchDiff >= 0) //We have all the bits we need in scratch
	{
		output = scratch >> scratchDiff; //Put the upper bits requested into output
		bitSelector = bitSelector >> (bitsPerWord - scratchDiff); //We only want the bottom bits for later use!
		scratch = scratch & bitSelector;
		scratchBits -= nobits;
	}
	else
	{
		if(index > 0 && read_index < index)
		{
			scratchDiff *= -1; //Make it positive
			output = scratch << scratchDiff; //Move the bits we currently have into the correct position in the output
			scratch = data[read_index]; //Load more bits from mem
			read_index += 1; //Move our data pointer to our next chunk of input
			scratchBits = sizeof(scratch) * 8; //We've now got an entire word in scratch
			uint32_t shiftRight = scratchBits - scratchDiff; //How far do we need to shift right to get the remaining bits we needed into the output
			output = output | (scratch >> shiftRight); //Write out the rest of the bits needed for the request
			scratch = (bitSelector >> scratchDiff) & scratch; //Get rid the bits we put into output
			scratchBits -= scratchDiff;
		}
		else
		{
			std::cout << "Not enough bits to satisfy request, returing garbage" << std::endl;
		}
	}
	return output;
}

void Buffer::flush()
{
	if(scratchBits > 0)
	{
		int freeBits = bitsPerWord - scratchBits;
		data[index] = scratch << freeBits;
		index += 1;
		scratch = 0;
		scratchBits = 0;
	}
}

uint64_t Buffer::getScratch()
{
	return scratch;
}

void Buffer::printData()
{
	for(int i = 0; i < index; i++)
	{
		std::cout << data[i] << std::endl;
	}
}

void Buffer::reset()
{
	scratch = 0;
	scratchBits = 0;
	index = 0;
	read_index = 0;
	memset(data, 0, sizeof(scratch) * len);
	len = 0;
}
