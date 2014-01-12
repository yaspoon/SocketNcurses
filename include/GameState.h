#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "Entity.h"
#include <map>
#include <vector>


class GameState
{
    public:
        GameState();
        virtual ~GameState();

        void addPlayer(int id);
        Player getPlayer(int id);
        std::vector<Player> getPlayers();
        void setPlayer(int id, Player player);
        std::vector<Ent> getGameUpdate();
        void setGameUpdate(std::vector<Ent> update);
    protected:
    private:
        std::map<int,Player> players;
};

#endif // GAMESTATE_H
