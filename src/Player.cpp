#include "Player.h"
#include <typeinfo>

Player::Player()
{
    x = 0;
    y = 0;
    character = '#';
}

Player::Player(Entity& ent)
{
    if(typeid(ent) == typeid(Player))
    {
        Player *tmp = dynamic_cast<Player*>(&ent);
        x = tmp->x;
        y = tmp->y;
        character = tmp->character;
    }
}

Player::Player(Ent copyEnt)
{
    x = copyEnt.data.player.x;
    y = copyEnt.data.player.y;
    character = copyEnt.data.player.character;
}

Player::~Player()
{
    //dtor
}

Ent Player::toEnt()
{
    Ent tmp;
    tmp.ent_type = ENT_PLAYER;
    tmp.data.player.character = character;
    tmp.data.player.x = x;
    tmp.data.player.y = y;

    return tmp;
}

/*Player& Player::operator=(const Player& inPlayer)
{

}

Player& Player::operator=(Player& inPlayer)
{

}*/
