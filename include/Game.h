#ifndef GAME_H
#define GAME_H

#include "Event.h"
#include "Players.h"

/*
 * Arbiter of all the game logic and data
 */
class Game
{
private:
	void handleKeyEvent(Event event);
	bool addPlayer();
public:
	Game();
	~Game();
	returnCodes_t update(long int time);
	template <typename Stream> bool serialise(Stream &stream);
};

#endif
