#include "Player.h"
#include <typeinfo>
#include "ReadBuffer.h"
#include "WriteBuffer.h"

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
}

template bool Player::serialise<ReadBuffer>(ReadBuffer&);
template bool Player::serialise<WriteBuffer>(WriteBuffer&);
