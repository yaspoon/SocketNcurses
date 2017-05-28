#include "Player.h"
#include <typeinfo>
#include "ReadBuffer.h"
#include "WriteBuffer.h"
#include "Stream.h"

Player::Player()
{
    x = 0;
    y = 0;
    character = '#';
}

Player::~Player()
{
    //dtor
}

/*Player& Player::operator=(const Player& inPlayer)
{

}

Player& Player::operator=(Player& inPlayer)
{

}*/

template <typename Stream> bool Player::serialise(Stream &stream)
{
	serialise_int(stream, x, INT32_MIN, INT32_MAX); 
	serialise_int(stream, y, INT32_MIN, INT32_MAX); 
	serialise_uint(stream, character, 0, UINT8_MAX);

	return true;
}

template bool Player::serialise<ReadBuffer>(ReadBuffer&);
template bool Player::serialise<WriteBuffer>(WriteBuffer&);
