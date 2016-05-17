#include "Game.h"
#include "GameObject.h"
#include "ObjectCode.h"

#include <Windows.h>

#include <iostream>

Game::Game(int numPlayers)
{
    m_pCurrentLevel = nullptr;
    m_numPlayers = numPlayers;
    m_pPlayers = new GameObject[numPlayers];
}

Game::~Game()
{
    delete m_pCurrentLevel;
    m_pCurrentLevel = nullptr;

    delete[] m_pPlayers;
    m_pPlayers = nullptr;
}

void Game::start()
{
    advanceLevel();
    for (;;) // forever
    {
        for (int p = 0; p < m_numPlayers; p++)
        {
            // Wait for input
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


DWORD WINAPI GameThreadEntryProc(void* pGameVoid)
{
    Game* game = reinterpret_cast<Game*>(pGameVoid);
    game->start();
    return 0;
}