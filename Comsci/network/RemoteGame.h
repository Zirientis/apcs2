#pragma once
#include "../Game.h"

class RemoteGame : private Game
{
private:
    RemoteGame(int numPlayers, GameType type, void(*getInput) (void*, Position*, const wchar_t*))
        : Game()
    {
        //
    }
};
