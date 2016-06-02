#include "Level.h"
#include "GameObject.h"
#include "Position.h"
#include "ObjectCode.h"
#include "LevelType.h"
#include "LevelGenerator.h"

Level::Level(int diff, LevelType type, const unsigned int seed)
{
    difficulty = diff;
    LevelGenerator::GenerateLevel(this, type, seed);
}

Level::~Level()
{
    delete[] m_pEntities;
    m_pEntities = nullptr;
    delete[] m_pFurnishings;
    m_pFurnishings = nullptr;
    delete[] m_pSurfaces;
    m_pSurfaces = nullptr;
    delete[] m_pOverlays;
    m_pOverlays = nullptr;
}

const GameObject* Level::GetOverlayAt(Position p)
{
    return m_pOverlays + (p.yTile * width + p.xTile);
}

const GameObject* Level::GetEntityAt(Position p)
{
    return m_pEntities + (p.yTile * width + p.xTile);
}

const GameObject* Level::GetFurnishingAt(Position p)
{
    return m_pFurnishings + (p.yTile * width + p.xTile);
}

const GameObject* Level::GetSurfaceAt(Position p)
{
    return m_pSurfaces + (p.yTile * width + p.xTile);
}

unsigned int Level::GetWidth()
{
    return width;
}

unsigned int Level::GetHeight()
{
    return height;
}
