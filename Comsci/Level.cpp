#include "Level.h"
#include "GameObject.h"
#include "Position.h"
#include "ObjectCode.h"


Level::Level(int)
{
    width = 20;
    height = 20;
    m_pEntities = new GameObject[width * height]; 
    m_pFurnishings = new GameObject[width * height];
    m_pSurfaces = new GameObject[width * height];
}

Level::~Level()
{
    delete[] m_pEntities;
    m_pEntities = nullptr;
    delete[] m_pFurnishings;
    m_pFurnishings = nullptr;
    delete[] m_pSurfaces;
    m_pSurfaces = nullptr;
}

GameObject Level::GetEntityAt(Position p)
{
    return m_pEntities[p.yTile * width + p.xTile];
}

GameObject Level::GetFurnishingAt(Position p)
{
    return m_pFurnishings[p.yTile * width + p.xTile];
}

GameObject Level::GetSurfaceAt(Position p)
{
    return m_pSurfaces[p.yTile * width + p.xTile];
}

unsigned int Level::GetWidth()
{
    return width;
}

unsigned int Level::GetHeight()
{
    return height;
}
