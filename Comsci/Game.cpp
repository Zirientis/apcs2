#include "Game.h"

#include "GameObject.h"
#include "ObjectCode.h"
#include "Position.h"

#include <Windows.h>

#include <vector>

Game::Game(int numPlayers, void (*getInputFunc) (void*, Position*))
{
    m_pCurrentLevel = nullptr;
    m_numPlayers = numPlayers;
    m_pPlayers = new GameObject[numPlayers];
    m_activePlayer = 0;
    getInput = getInputFunc;
    synchronizedPos.xTile = 0;
    synchronizedPos.yTile = 0;
    inputEvent = CreateEvent(NULL, true, false, LPCWSTR(u"Game Input Ready"));
}

Game::~Game()
{
    delete m_pCurrentLevel;
    m_pCurrentLevel = nullptr;

    delete[] m_pPlayers;
    m_pPlayers = nullptr;
    getInput = nullptr;
    synchronizedPos.xTile = 0;
    synchronizedPos.yTile = 0;
    CloseHandle(inputEvent);
    inputEvent = nullptr;
}

void Game::start()
{
    advanceLevel();
    for (;;) // forever
    {
        for (m_activePlayer = 0; m_activePlayer < m_numPlayers; m_activePlayer++)
        {
            // Wait for input
            Position p;
            getInput(this, &p);
        }
        // AI logic runs here
    }
}

void Game::advanceLevel()
{
    int oldDiff = 0;
    if (m_pCurrentLevel)
    {
        oldDiff = m_pCurrentLevel->difficulty;
        delete m_pCurrentLevel;
    }
    m_pCurrentLevel = new Level(oldDiff + 1);
}

int Game::getActivePlayer() // main thread, and only when locked
{
    return m_activePlayer;
}
/*
std::vector<Action> Game::getPotentialPlayerActions()
{

}
*/
bool Game::MaybeSendPosition(Position pos)
{
    // First check to ensure the game thread is done using the position.
    // It should be nonsignalled. If signaled, game thread is still active.
    if (WaitForSingleObject(inputEvent, 0) != WAIT_TIMEOUT)
    {
        //DebugBreak();
        return false;
    }
    synchronizedPos.xTile = pos.xTile;
    synchronizedPos.yTile = pos.yTile;
    SetEvent(inputEvent);
    return true;
}

void Game::DefaultMemberGetInput(Position* outPos) // game thread only!
{
    WaitForSingleObject(inputEvent, INFINITE);
    outPos->xTile = synchronizedPos.xTile;
    outPos->yTile = synchronizedPos.yTile;
    ResetEvent(inputEvent);
}

void DefaultInputFunc(void* pGameVoid, Position* outPos) // game thread only!
{
    Game* game = reinterpret_cast<Game*>(pGameVoid);
    game->DefaultMemberGetInput(outPos);
}

DWORD WINAPI GameThreadEntryProc(void* pGameVoid)
{
    Game* game = reinterpret_cast<Game*>(pGameVoid);
    game->start();
    return 0;
}
