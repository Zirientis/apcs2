#include "Level.h"
#include "GameObject.h"
#include "Position.h"
#include "ObjectCode.h"


Level::Level(int diff)
{
    difficulty = diff;
    unsigned int segments = BASE_SEGMENTS_PER_LEVEL + diff / INCR_SEGMENTS_AFTER_COUNT;
    width = segments * BASE_SEGMENT_LENGTH;
    height = segments * BASE_SEGMENT_LENGTH;
    m_pEntities = new GameObject[width * height];
    m_pFurnishings = new GameObject[width * height];
    m_pSurfaces = new GameObject[width * height];
    m_pOverlays = new GameObject[width * height];
    for (unsigned int row = 0; row < height; row += BASE_SEGMENT_LENGTH)
    {
        for (unsigned int col = 0; col < width; col += BASE_SEGMENT_LENGTH)
        {
            drawRect(m_pSurfaces, col, row, col + BASE_SEGMENT_LENGTH,
                row + BASE_SEGMENT_LENGTH, WALL_MARIO);
            fillRect(m_pSurfaces, col + 1, row + 1, col + BASE_SEGMENT_LENGTH - 1,
                row + BASE_SEGMENT_LENGTH - 1, FLOOR_DIRT);
            unsigned int holePunchOffset = BASE_SEGMENT_LENGTH / 2;
            m_pSurfaces[(row + holePunchOffset) * width + col].setCode(FLOOR_DIRT);
            m_pSurfaces[(row + holePunchOffset) * width + col + BASE_SEGMENT_LENGTH - 1].setCode(FLOOR_DIRT);
            m_pSurfaces[row * width + col + holePunchOffset].setCode(FLOOR_DIRT);
            m_pSurfaces[(row + BASE_SEGMENT_LENGTH - 1) * width + col + holePunchOffset].setCode(FLOOR_DIRT);

            m_pEntities[(row + holePunchOffset) * width + col + holePunchOffset].setCode(MONST_SPIDER);
        }
    }
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

void Level::drawRect(GameObject* arr, unsigned int left, unsigned int top,
    unsigned int right, unsigned int bottom, ObjectCode code)
{
    // Go top, bottom, left, right. On sides, remember that corners are done.
    fillRect(arr, left, top, right, top + 1, code);
    fillRect(arr, left, bottom - 1, right, bottom, code);
    fillRect(arr, left, top + 1, left + 1, bottom - 1, code);
    fillRect(arr, right - 1, top + 1, right, bottom - 1, code);
}

void Level::fillRect(GameObject* arr, unsigned int left, unsigned int top,
    unsigned int right, unsigned int bottom, ObjectCode code)
{
    for (unsigned int r = top; r < bottom; r++)
    {
        for (unsigned int c = left; c < right; c++)
        {
            arr[r * width + c].setCode(code);
        }
    }
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
