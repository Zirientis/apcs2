#pragma once
#include "Level.h"

class Game
{
private:
    Level* m_pCurrentLevel;
    GameObject* m_pPlayers;
    int m_numPlayers;

    void advanceLevel();
public:
    Game(int);
    ~Game();
    void start();
};