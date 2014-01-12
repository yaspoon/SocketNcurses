#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"


class Player : public Entity
{
    public:
        Player();
        Player(Entity& ent);
        Player(Ent copyEnt);
        Ent toEnt();
        virtual ~Player();
        int x;
        int y;
        char character;

        //Player& operator=(const Player& inPlayer);
        //Player& operator=(Player& inPlayer);
    protected:
    private:
};

#endif // PLAYER_H
