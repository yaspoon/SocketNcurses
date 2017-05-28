#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"


class Player : public Entity
{
    public:
        Player();
        virtual ~Player();
	template <typename Stream> bool serialise(Stream &stream);
        int x;
        int y;
        char character;

        //Player& operator=(const Player& inPlayer);
        //Player& operator=(Player& inPlayer);
    protected:
    private:
};

#endif // PLAYER_H
