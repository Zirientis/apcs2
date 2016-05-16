#pragma once
#include "GameObject.h"
class Level
{
private:
    GameObject* m_pFixtures;
    GameObject* m_pEntities;

    int difficulty;
public:
    Level(int);
    ~Level();
};

