#include "Players.h"
#include <typeinfo>
#include "ReadBuffer.h"
#include "WriteBuffer.h"
#include "Stream.h"

Players::Players()
{
	memset(positions, 0, sizeof(positions));
	memset(velocitys, 0, sizeof(velocitys));
	memset(characters, 0, sizeof(characters));
}

Players::~Players()
{
    //dtor
}

/*Player& Player::operator=(const Player& inPlayer)
{

}

Player& Player::operator=(Player& inPlayer)
{

}*/

template <typename Stream> bool Players::serialise(Stream &stream)
{
	serialise_uint(stream, numPlayers, 0, UINT32_MAX);
	serialise_array(stream, (char*)positions, sizeof(positions));
	serialise_array(stream, (char*)velocitys, sizeof(velocitys));
	serialise_array(stream, (char*)characters, sizeof(characters));

	return true;
}

template bool Players::serialise<ReadBuffer>(ReadBuffer&);
template bool Players::serialise<WriteBuffer>(WriteBuffer&);
