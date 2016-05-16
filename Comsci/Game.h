#pragma once
#include "Level.h"

class Game
{
private:
    Level* m_pCurrentLevel;
    GameObject* m_pPlayers;

    void advanceLevel();
public:
    Game(int);
    ~Game();
    void start();
};