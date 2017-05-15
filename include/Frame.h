#ifndef FRAME_H
#define FRAME_H

#include "Buffer.h"

class Frame
{
private:
	uint8_t type; //It's really a Net_type enum
	uint32_t id;
	uint32_t num;
public:
	Frame();
	Frame(uint8_t newType, uint32_t newNum);
	~Frame();
	uint8_t getType();
	uint32_t getNum();
	uint32_t getId();
	void setType(uint8_t newType);
	void setNum(uint32_t newNum);
	void setId(uint32_t newId);
	template <typename Stream> bool serialise(Stream &stream);

	enum
	{
		TYPE_START = 0,
		ACK,
		CONNECTION_REQUEST,
		CONNECTION_REQUEST_ACK,
		CONNECTION_ACCEPTED,
		CONNECTION_ACCEPTED_ACK,
		DATA,
		ERROR,
		TYPE_END,
	};
};

#endif
