#include "Game.h"
#include "GameObject.h"
#include "ObjectCode.h"
#include "Position.h"

#include <Windows.h>

#include <vector>

Game::Game(int numPlayers, Position (*getInputFunc) (void))
{
    m_pCurrentLevel = nullptr;
    m_numPlayers = numPlayers;
    m_pPlayers = new GameObject[numPlayers];
    getInput = getInputFunc;
}

Game::~Game()
{
    delete m_pCurrentLevel;
    m_pCurrentLevel = nullptr;

    delete[] m_pPlayers;
    m_pPlayers = nullptr;
    getInput = nullptr;
}

void Game::start()
{
    advanceLevel();
    for (;;) // forever
    {
        for (int p = 0; p < m_numPlayers; p++)
        {
            // Wait for input
            getInput();
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

int Game::getActivePlayer()
{
    // lock
}

std::vector<Action> Game::getPotentialPlayerActions()
{

}

DWORD WINAPI GameThreadEntryProc(void* pGameVoid)
{
    Game* game = reinterpret_cast<Game*>(pGameVoid);
    game->start();
    return 0;
}