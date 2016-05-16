#include "Game.h"
#include "GameObject.h"
Game::Game(int numPlayers)
{
    m_pPlayers = FIXME
    m_pCurrentLevel = nullptr;
}

Game::~Game()
{
    delete m_pCurrentLevel;
    m_pCurrentLevel = nullptr;
}

void Game::start()
{
    advanceLevel();
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