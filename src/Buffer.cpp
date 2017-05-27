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
	bitsWritten = 0;
	status = OKAY;
}

Buffer::Buffer(const Buffer &copy)
{
	len = copy.len;
	index = copy.index;
	read_index = copy.read_index;
	data = new uint64_t[len];
	std::copy(&copy.data[0], &copy.data[0] + len, &data[0]);
	scratch = copy.scratch;
	scratchBits = copy.scratchBits;
	bitsWritten = copy.bitsWritten;
	status = OKAY;
}

Buffer::~Buffer()
{
	delete[] data;
}

Buffer &Buffer::operator=(const Buffer &copy)
{
	if(&copy != this)
	{
		len = copy.len;
		index = copy.index;
		read_index = copy.read_index;
		data = new uint64_t[len];
		std::copy(&copy.data[0], &copy.data[0] + len, &data[0]);
		scratch = copy.scratch;
		scratchBits = copy.scratchBits;
		bitsWritten = copy.bitsWritten;
	}
	return *this;
}

/* 
 * The only time this should fail is if we run out of memory
 * on reallocing the data buffer, so retval should pretty much
 * always be true
 */
returnCodes_t Buffer::write(uint64_t input, uint32_t nobits)
{
	returnCodes_t retval = OKAY;
	uint64_t selectBits = 0;

	int newTotalBits = scratchBits + nobits;
	if(newTotalBits > bitsPerWord) //More bits then we can fit in a word
	{
		int freeBits = (bitsPerWord - scratchBits); //Number of bits left before this word is full
		int leftOver = nobits - freeBits; //Number of bits left over in the input
		if(scratchBits < bitsPerWord) {
			scratch = scratch | (input << (bitsPerWord - freeBits)); //Add input bits that will fit to the end of scratch
		}

		if(index < len) //Do we have a large enough buffer to write this out?
		{
			data[index] = scratch; //Flush word to buffer
		}
		else //No we don't so allocate some extra
		{
			std::cout << "Index:" << index << "Len:" << len << std::endl;
			int new_len = len * 2;
			typeof(scratch) *new_data = new uint64_t[new_len];
			if(new_data != NULL)
			{
				memset(new_data, 0, sizeof(scratch) * new_len);
				memcpy(new_data, data, sizeof(scratch) * len);
				delete[] data;
				data = new_data;
				len = new_len;

				data[index] = scratch; //Flush word to buffer
			}
			else
			{
				std::cout << "Failed to extend buffer, cannot write out data" << std::endl;
				status = ERROR;
				retval = ERROR;
			}
		}
		index += 1;
		bitsWritten += sizeof(scratch) * 8;
		scratch = 0;

		if(leftOver >= bitsPerWord) //Shifting by number of bits in a word or greater is undefined behaviour
		{
			selectBits = ~selectBits;
		}
		else
		{
			selectBits = ~((~selectBits) << leftOver);
		}
		scratch = input >> freeBits; //Get rid of bits we put into buffer
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
		//scratch = scratch | ((selectBits & input) << scratchBits); //Shift input to add it on the end of current scratch bits
		scratch = scratch | (input << scratchBits); //Shift input to add it on the end of current scratch bits
		scratchBits += nobits;
	}

	return retval;
}

uint64_t Buffer::read(uint32_t nobits)
{
	typeof(scratch) output = 0;
	int32_t scratchDiff = scratchBits - nobits;
	typeof(scratch) bitSelector = ~0; //Set a word to all 1 bits
	uint32_t shiftRight = bitsPerWord - nobits; //How far do we need to shift bitSelector right to get the bits we need into the output
	if(scratchDiff >= 0) //We have all the bits we need in scratch
	{
		output = scratch & (bitSelector >> shiftRight); //Select only bottom bits that were asked for
		scratch = scratch >> nobits;
		scratchBits -= nobits;
	}
	else
	{
		if(index > 0 && read_index < index)
		{
			scratchDiff *= -1; //Make it positive
			output = scratch; //Move the bits we currently have into the output
			scratch = data[read_index]; //Load more bits from mem
			uint32_t bitsLeftWritten = bitsWritten - (read_index * sizeof(scratch) * 8); //How many bits do we have left written in the buffer.
			if(bitsLeftWritten < bitsPerWord) {
				output = output | (scratch << scratchBits); //Write out the rest of the bits needed for the request
				scratchBits = bitsLeftWritten - scratchDiff;
				scratch = scratch >> scratchDiff; //Get rid of bits we just wrote out
			} else {
				output = output | ((scratch & (bitSelector >> (bitsPerWord - nobits))) << scratchBits); //Write out the rest of the bits needed for the request
				scratchBits = sizeof(scratch) * 8 - scratchDiff; //We've now got an entire word in scratch
				if(scratchDiff < bitsPerWord) {
					scratch = scratch >> scratchDiff; //Get rid the bits we put into output
				} else {
					scratch = 0;
				}
			}
			read_index += 1; //Move our data pointer to our next chunk of input
		}
		else
		{
			std::cout << "Not enough bits to satisfy request, returing garbage" << std::endl;
			status = ERROR;
		}
	}
	return output;
}

void Buffer::flush()
{
	if(scratchBits > 0)
	{
		int freeBits = bitsPerWord - scratchBits;
		data[index] = scratch;// << freeBits;
		bitsWritten += scratchBits;
		index += 1;
		scratch = 0;
		scratchBits = 0;
	}
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
	bitsWritten = 0;
	index = 0;
	read_index = 0;
	memset(data, 0, sizeof(scratch) * len);
}

returnCodes_t Buffer::writeBool(bool input)
{
	return write((uint64_t)input, 1);
}

returnCodes_t Buffer::writeInt8(int8_t input)
{
	return write((uint64_t)input, 8);
}

returnCodes_t Buffer::writeUint8(uint8_t input)
{
	return write((uint64_t)input, 8);
}

returnCodes_t Buffer::writeInt16(int16_t input)
{
	return write((uint64_t)input, 16);
}

returnCodes_t Buffer::writeUint16(uint16_t input)
{
	return write((uint64_t)input, 16);
}

returnCodes_t Buffer::writeEnum(uint32_t input) 
{
	return write((uint64_t)input, 32);
}

returnCodes_t Buffer::writeInt32(int32_t input)
{
	return write((uint64_t)input, 32);
}

returnCodes_t Buffer::writeUint32(uint32_t input)
{
	return write((uint64_t)input, 32);
}

returnCodes_t Buffer::writeInt64(int64_t input)
{
	return write((uint64_t)input, 64);
}

returnCodes_t Buffer::writeUint64(uint64_t input)
{
	return write((uint64_t)input, 64);
}

returnCodes_t Buffer::writeBuffer(Buffer input)
{
	returnCodes_t retval = OKAY;
	int words = input.bitsWritten / bitsPerWord;
	for(int i = 0; i < words && retval; i++)
	{
		retval = write(input.data[i], bitsPerWord);
	}

	int leftOver = input.bitsWritten % bitsPerWord;
	if(leftOver != 0 && retval) {
		retval = write(input.data[words], leftOver);
	}

	return retval;
}

bool Buffer::readBool()
{
	bool retval = (bool)read(1);
	return retval;
}

int8_t Buffer::readInt8()
{
	int8_t retval = (int8_t)read(8);
	return retval;
}

uint8_t Buffer::readUint8()
{
	uint8_t retval = (uint8_t)read(8);
	return retval;
}

int16_t Buffer::readInt16()
{
	int16_t retval = (int16_t)read(16);
	return retval;
}

uint16_t Buffer::readUint16()
{
	uint16_t retval = (uint16_t)read(16);
	return retval;
}

uint32_t Buffer::readEnum()
{
	uint32_t retval = (uint32_t)read(32);
	return retval;
}

int32_t Buffer::readInt32()
{
	int32_t retval = (int32_t)read(32);
	return retval;
}

uint32_t Buffer::readUint32()
{
	uint32_t retval = (uint32_t)read(32);
	return retval;
}

int64_t Buffer::readInt64()
{
	int64_t retval = (int64_t)read(64);
	return retval;
}

uint64_t Buffer::readUint64()
{
	uint64_t retval = (uint64_t)read(64);
	return retval;
}

int Buffer::bitsLeft()
{
	int retval = bitsWritten - (read_index * sizeof(scratch) * 8);
	return (retval > 0) ? retval : 0;
}

int Buffer::totalBits()
{
	return bitsWritten;
}

int Buffer::bytesLeft()
{
	int retval = totalBytes() - (read_index * sizeof(scratch));
	return (retval > 0) ? retval : 0; //Not sure this will work. Reasonably sure retval will giant because size_t is unsigned...
}

int Buffer::totalBytes()
{
	int retval = ((bitsWritten / 8) + (scratchBits / 8));
	if((bitsWritten % 8) != 0) {
		retval += 1;
	}
	if((scratchBits % 8) != 0) {
		retval += 1;
	}
	return retval;
}

uint64_t *Buffer::bytes()
{
	return data;
}

void Buffer::fillFromBuffer(uint64_t *buf, size_t nbytes)
{
	scratch = 0;
	scratchBits = 0;
	read_index = 0;
	int nwords = 0;
	if(nbytes % sizeof(scratch) != 0) {
		nwords = (nbytes / sizeof(scratch)) + 1;
	} else {
		nwords = (nbytes / sizeof(scratch));
	}
	if(nwords > len) {
		len = nwords;
		delete[] data;
		data = new uint64_t[len];
	}
	index = len;
	bitsWritten = nbytes * 8; //This isn't perfectly accurate
	memcpy(data, buf, sizeof(scratch) * nwords);
}

returnCodes_t Buffer::getStatus()
{
	return status;
}
