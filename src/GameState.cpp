#include "GameState.h"
#include <typeinfo>

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

std::vector<Ent> GameState::getGameUpdate()
{
    std::vector<Ent> retVal;

    for(int i = 0; i < players.size(); i++)
    {
        Ent tmp = players[i].toEnt();
        retVal.push_back(tmp);
    }

    return retVal;
}

void GameState::setGameUpdate(std::vector<Ent> update)
{
    for(int i = 0; i < update.size(); i++)
    {
        Player newPlayer(update[i]);
        players[i] = newPlayer;
    }
}
