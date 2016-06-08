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

Level::Level(GameObject* over, GameObject* ent, GameObject* furn, GameObject* surf, int diff, unsigned int w, unsigned int h)
    : m_pOverlays(over),
      m_pEntities(ent),
      m_pFurnishings(furn),
      m_pSurfaces(surf),
      difficulty(diff),
      width(w),
      height(h)
{}

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

void Level::MarkAllEntitiesReady()
{
    for (unsigned int i = 0; i < width * height; i++)
    {
        GameObject* npc = m_pEntities + i;
        npc->setActionPerformed(false);
    }
}

int Level::GetDifficulty()
{
    return difficulty;
}

void Level::SetDifficulty(int d)
{
    difficulty = d;
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

// ----- HERE BE DRAGONS -----

GameObject* Level::GetOverlays()
{
    return m_pOverlays;
}

GameObject* Level::GetEntities()
{
    return m_pEntities;
}

GameObject* Level::GetFurnishings()
{
    return m_pFurnishings;
}

GameObject* Level::GetSurfaces()
{
    return m_pSurfaces;
}