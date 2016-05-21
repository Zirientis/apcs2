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
    Position* m_pPlayerPositions; // For Game bookkeeping
    int m_numPlayers;
    int m_activePlayer;
    bool playersPlaced;
    Position synchronizedPos;
    HANDLE inputEvent;
    void (*getInput) (void*, Position*);

    bool moveEntity(Position start, Position end);
    bool placeEntity(GameObject& templateObj, Position pos, bool force);
    void advanceLevel();
public:
    Game(int, void (*getInput) (void*, Position*));
    ~Game();
    void start();

    int getActivePlayer();
    //std::vector<Action> getPotentialPlayerActions();
    void DefaultMemberGetInput(Position*); // game thread only!
    bool MaybeSendPosition(Position); // Returns whether the position was set

    const GameObject* GetEntityAt(Position);
    const GameObject* GetFurnishingAt(Position);
    const GameObject* GetSurfaceAt(Position);
    const GameObject* GetOverlayAt(Position);
    unsigned int GetWidth();
    unsigned int GetHeight();
};

void DefaultInputFunc(void*, Position*); // Game thread only!
DWORD WINAPI GameThreadEntryProc(void*);