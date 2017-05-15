#include "GameState.h"
#include <typeinfo>
#include <iostream>

#include "ReadBuffer.h"
#include "WriteBuffer.h"

GameState::GameState()
{
    //ctor
}

GameState::~GameState()
{
    //dtor
}

void GameState::addPlayer(int id)
{
    players[id] = Player();
}

Player GameState::getPlayer(int id)
{
    return players[id];
}

std::vector<Player> GameState::getPlayers()
{
    std::vector<Player> retVal;

    for(int i = 0; i < players.size(); i++)
    {
        Player tmp = players[i];

        retVal.push_back(tmp);
    }

    return retVal;
}

void GameState::setPlayer(int id, Player player)
{
    players[id] = player;
}

template <typename Stream> bool GameState::serialise(Stream &stream)
{
	bool retval = true;
	uint64_t size = players.size();
	serialise_uint(stream, size, 0, MAX_PLAYERS);
	for(int i = 0; i < players.size() && retval; i++) {
		if(!players[i].serialise(stream)) {
			std::cout << "GameState:Serialising player " << i << " failed, aborting" << std::endl;
			retval = false;
		}
	}

    return retval;
}

template bool GameState::serialise<ReadBuffer>(ReadBuffer &);
template bool GameState::serialise<WriteBuffer>(WriteBuffer &);
