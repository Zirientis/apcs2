#pragma once
#include "GameObject.h"
#include "Position.h"
class Level
{
    friend class Game;
private:
    GameObject* m_pEntities;
    GameObject* m_pFurnishings;
    GameObject* m_pSurfaces;

    int difficulty;
    unsigned int width, height;
public:
    Level(int);
    ~Level();
    GameObject GetEntityAt(Position);
    GameObject GetFurnishingAt(Position);
    GameObject GetSurfaceAt(Position);
    unsigned int GetWidth();
    unsigned int GetHeight();
};
