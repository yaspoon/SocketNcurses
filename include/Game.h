#ifndef GAME_H
#define GAME_H

#include "Event.h"
#include "GameState.h"

/*
 * Arbiter of all the game logic and data
 */
class Game
{
private:
	GameState state;
	void handleKeyEvent(Event event);
	bool addPlayer();
public:
	Game();
	~Game();
	template <typename Stream> bool serialise(Stream &stream);
};

#endif
