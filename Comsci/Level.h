#pragma once
#include "GameObject.h"
class Level
{
    friend class Game;
private:
    GameObject* m_pFixtures;
    GameObject* m_pEntities;

    int difficulty;
public:
    Level(int);
    ~Level();
};
