#pragma once
#include "Level.h"
#include "Position.h"
#include <Windows.h>

#include <vector>

class Game
{
private:
    Level* m_pCurrentLevel;
    GameObject* m_pPlayers;
    int m_numPlayers;

    void advanceLevel();
    Position (*getInput) (void);
public:
    Game(int, Position (*getInput) (void));
    ~Game();
    void start();

    int getActivePlayer();
    std::vector<Action> getPotentialPlayerActions();
};

DWORD WINAPI GameThreadEntryProc(void*);