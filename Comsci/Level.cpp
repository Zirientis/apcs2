#include "Level.h"
#include "GameObject.h"
#include "Position.h"
#include "ObjectCode.h"

#include <random>

Level::Level(int diff, std::mt19937 rng)
{
    difficulty = diff;
    unsigned int segments = BASE_SEGMENTS_PER_LEVEL;
    if (INCR_SEGMENTS_AFTER_COUNT)
        segments += diff / INCR_SEGMENTS_AFTER_COUNT;
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
                row + BASE_SEGMENT_LENGTH, GameObject(WALL_MARIO, -1));
            fillRect(m_pSurfaces, col + 1, row + 1, col + BASE_SEGMENT_LENGTH - 1,
                row + BASE_SEGMENT_LENGTH - 1, GameObject(FLOOR_DIRT, -1));
            unsigned int holePunchOffset = BASE_SEGMENT_LENGTH / 2;
            m_pSurfaces[(row + holePunchOffset) * width + col].setCode(FLOOR_DIRT);
            m_pSurfaces[(row + holePunchOffset) * width + col + BASE_SEGMENT_LENGTH - 1].setCode(FLOOR_DIRT);
            m_pSurfaces[row * width + col + holePunchOffset].setCode(FLOOR_DIRT);
            m_pSurfaces[(row + BASE_SEGMENT_LENGTH - 1) * width + col + holePunchOffset].setCode(FLOOR_DIRT);

            m_pEntities[(row + holePunchOffset) * width + col + holePunchOffset] = GameObject(SPAWN_SPIDER, 1);
        }
    }
    drawRect(m_pSurfaces, 0, 0, width, height, GameObject(WALL_BRICK, -1));
    unsigned int row, col;
    ObjectCode surf;
    do
    {
        row = rng() % height;
        col = rng() % width;
        surf = m_pSurfaces[row * width + col].getCode();
    } while (m_pEntities[row * width + col].getCode() != ObjectCode::NONE ||
        (surf >= MIN_WALL && surf <= MAX_WALL));
    m_pFurnishings[row * width + col] = GameObject(STAIRS, -1);
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
    unsigned int right, unsigned int bottom, GameObject& templ)
{
    // Go top, bottom, left, right. On sides, remember that corners are done.
    fillRect(arr, left, top, right, top + 1, templ);
    fillRect(arr, left, bottom - 1, right, bottom, templ);
    fillRect(arr, left, top + 1, left + 1, bottom - 1, templ);
    fillRect(arr, right - 1, top + 1, right, bottom - 1, templ);
}

void Level::fillRect(GameObject* arr, unsigned int left, unsigned int top,
    unsigned int right, unsigned int bottom, GameObject& templ)
{
    for (unsigned int r = top; r < bottom; r++)
    {
        for (unsigned int c = left; c < right; c++)
        {
            arr[r * width + c] = GameObject(templ);
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
