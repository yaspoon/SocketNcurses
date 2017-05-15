#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "Entity.h"
#include <map>
#include <vector>
#include "Stream.h"

class GameState
{
    public:
	    static const int MAX_PLAYERS = 128;
        GameState();
        virtual ~GameState();

        void addPlayer(int id);
        Player getPlayer(int id);
        std::vector<Player> getPlayers();
        void setPlayer(int id, Player player);
	template <typename Stream> bool serialise(Stream &stream);
    protected:
    private:
        std::map<int,Player> players;
};

#endif // GAMESTATE_H
