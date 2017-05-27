#include "Game.h"
#include <iostream>

#include "WriteBuffer.h"
#include "ReadBuffer.h"

#include "Log.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::addPlayer()
{
	STUB << "Game::addPlayer" << STUB_END;
	bool retVal = false;

	return retVal;
}

void Game::handleKeyEvent(Event event)
{
    switch(event.key.key_type)
    {
        case Event::EVENT_KEY_DOWN:
        {
            switch(event.key.sym)
            {
                case Event::KEY_A:
                {
                    Player thePlayer = state.getPlayer(event.id);
                    thePlayer.x -= 1;
                    state.setPlayer(event.id, thePlayer);
                    DEBUG << "Client[" << event.id << "].CL_Player.x=" << thePlayer.x << DEBUG_END;
                }
                break;
                case Event::KEY_W:
                {
                    Player thePlayer = state.getPlayer(event.id);
                    thePlayer.y -= 1;
                    state.setPlayer(event.id, thePlayer);
                    DEBUG << "Client[" << event.id << "].CL_Player.y=" << thePlayer.y << DEBUG_END;
                }
                break;
                case Event::KEY_S:
                {
                    Player thePlayer = state.getPlayer(event.id);
                    thePlayer.y += 1;
                    state.setPlayer(event.id, thePlayer);
                    DEBUG << "Client[" << event.id << "].CL_Player.y=" << thePlayer.y << DEBUG_END;
                }
                break;
                case Event::KEY_D:
                {
                    Player thePlayer = state.getPlayer(event.id);
                    thePlayer.x += 1;
                    state.setPlayer(event.id, thePlayer);
                    DEBUG << "Client[" << event.id << "].CL_Player.x=" << thePlayer.x << DEBUG_END;
                }
                break;
            }
        }
        break;
    }
}

template <typename Stream> bool Game::serialise(Stream &stream)
{
	STUB << "Game::serialise" << STUB_END;
	return state.serialise(stream);
}

template bool Game::serialise<WriteBuffer>(WriteBuffer&);
template bool Game::serialise<ReadBuffer>(ReadBuffer&);
