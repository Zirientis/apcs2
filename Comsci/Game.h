#pragma once
#include "Level.h"
#include "Position.h"
#include <Windows.h>

#include <vector>


class Game
{
private:
    Level* m_pCurrentLevel;
    //GameObject* m_pPlayers;
    int m_numPlayers;
    int m_activePlayer;
    void (*getInput) (void*, Position*);
    volatile Position synchronizedPos;
    HANDLE inputEvent;

    void advanceLevel();
public:
    Game(int, void (*getInput) (void*, Position*));
    ~Game();
    void start();

    int getActivePlayer();
    //std::vector<Action> getPotentialPlayerActions();
    void DefaultMemberGetInput(Position*); // game thread only!
    bool MaybeSendPosition(Position); // Returns whether the position was set

    GameObject GetEntityAt(Position);
    GameObject GetFurnishingAt(Position);
    GameObject GetSurfaceAt(Position);
    unsigned int GetWidth();
    unsigned int GetHeight();
};

void DefaultInputFunc(void*, Position*); // Game thread only!
DWORD WINAPI GameThreadEntryProc(void*);