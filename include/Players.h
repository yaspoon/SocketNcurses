#ifndef PLAYER_H
#define PLAYER_H

#include "Entitys.h"

const uint32_t MAX_PLAYERS = 32;

class Players : public Entitys
{
    public:
        Players();
        virtual ~Players();
	template <typename Stream> bool serialise(Stream &stream);

        //Player& operator=(const Player& inPlayer);
        //Player& operator=(Player& inPlayer);
    protected:
    private:
	uint32_t numPlayers;
	Vec2D positions[MAX_PLAYERS];
	Vec2D velocitys[MAX_PLAYERS];
        char characters[MAX_PLAYERS];
};

#endif // PLAYER_H
