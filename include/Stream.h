#ifndef STREAM_H
#define STREAM_H

#include <assert.h>

template <uint64_t x> struct PopCount
{
    enum { a = x - ( ( x >> 1 ) & 0x5555555555555555 ),
           b = ( ( ( a >> 2 ) & 0x3333333333333333 ) + ( a & 0x3333333333333333 ) ),
           c = ( ( ( b >> 4 ) + b ) & 0x0f0f0f0f0f0f0f0f ),
           d = c + ( c >> 8 ),
           e = d + ( d >> 16 ),
           f = e + ( e >> 32 ),
    result = f & 0x0000003f }; 
};

template <uint64_t x> struct Log2
{
	enum { a = x | ( x >> 1 ),
		b = a | ( a >> 2 ),
		c = b | ( b >> 4 ),
		d = c | ( c >> 8 ),
		e = d | ( d >> 16),
		f = e | ( e >> 32),
		g = f >> 1,
		result = PopCount<g>::result
		};
};

template <int64_t min, int64_t max> struct BitsRequired
{
	static const uint32_t result = (min == max) ? 0 : (Log2<uint64_t(max-min)>::result + 1);
};

#define bits_required(min, max) BitsRequired<min, max>::result

#define serialise_int(stream, value, min, max)					\
	do									\
	{									\
		assert(min < max);						\
		int64_t int64_value;						\
		if(Stream::isWriting) {						\
			assert(value >= min);					\
			assert(value <= max);					\
			int64_value = (typeof(int64_value))value;		\
		}								\
		if(!stream.SerialiseInt(int64_value, bits_required(min, max))) {		\
			return false;						\
		}								\
		if(Stream::isReading) {						\
			value = int64_value;					\
			if(value < min || value > max) {			\
				return false;					\
			}							\
		}								\
	} while(0)

#define serialise_uint(stream, value, min, max)					\
	do									\
	{									\
		assert(min < max);						\
		uint64_t uint64_value;						\
		if(Stream::isWriting) {						\
			assert(value >= min);					\
			assert(value <= max);					\
			uint64_value = (typeof(uint64_value))value;		\
		}								\
		if(!stream.SerialiseUint(uint64_value, bits_required(min, max))) {		\
			return false;						\
		}								\
		if(Stream::isReading) {						\
			value = uint64_value;					\
			if(value < min || value > max) {			\
				return false;					\
			}							\
		}								\
	} while(0)

#define serialise_float(stream, value)						\
	do									\
	{									\
		union FloatToInt						\
		{								\
			float f;						\
			uint32_t i;						\
		};								\
		uint64_t uint64_value;						\
		uint32_t noBitsInFloat = sizeof(float) * 8;			\
		FloatToInt fti;							\
		if(Stream::isWriting) {						\
			fti.f = value;						\
			uint64_value = (typeof(uint64_value))fti.i;		\
		}								\
		if(!stream.SerialiseBits((uint64_t)fti.i, noBitsInFloat)) {	\
			return false						\
		}								\
		if(Stream::isReading) {						\
			value = fti.f;						\
		}								\
	} while(0)

#define serialise_buffer(stream, data)						\
	do									\
	{									\
		if(!stream.SerialiseBuffer(data)) {				\
			return false;						\
		}								\
	} while(0)

#define serialise_array(stream, array, nobytes)					\
	do									\
	{									\
		if(!stream.ByteAlign()) {					\
			return false;						\
		}								\
		if(!stream.SerialiseArray(array, nobytes)) {			\
			return false;						\
		}								\
	} while(0)
#endif
