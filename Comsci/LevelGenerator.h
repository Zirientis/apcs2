#pragma once
#include "LevelType.h"
#include <random>
#include <limits>
class LevelGenerator
{
public:
    static void GenerateLevel(Level* l, LevelType type)
    {
        switch (type)
        {
        case LT_SPIDER:
            GenerateSpiderLevel(l);
            return;
        case LT_SNEK:
            GenerateSnekLevel(l);
            return;
        case LT_CLASSIC_1:
            //GenerateClassic1Level(l);
            return;
        default:
            __debugbreak();
            // Unknown level type!
        }
    }

    static void GenerateSnekLevel(Level* l)
    {
        std::random_device rd;
        std::mt19937 rng = std::mt19937(rd());
        unsigned int segments = BASE_SEGMENTS_PER_LEVEL;
        if (INCR_SEGMENTS_AFTER_COUNT != 0)
            segments += l->difficulty / INCR_SEGMENTS_AFTER_COUNT;
        l->width = (segments - 1) * (BASE_SEGMENT_LENGTH - 1) + BASE_SEGMENT_LENGTH;
        l->height = (segments - 1) * (BASE_SEGMENT_LENGTH - 1) + BASE_SEGMENT_LENGTH;

        l->m_pEntities = new GameObject[l->width * l->height];
        l->m_pFurnishings = new GameObject[l->width * l->height];
        l->m_pSurfaces = new GameObject[l->width * l->height];
        l->m_pOverlays = new GameObject[l->width * l->height];
        for (unsigned int row = 0; row < l->height; row += (BASE_SEGMENT_LENGTH - 1))
        {
            if (row + BASE_SEGMENT_LENGTH - 1 >= l->height)
                continue;
            for (unsigned int col = 0; col < l->width; col += (BASE_SEGMENT_LENGTH - 1))
            {
                if (col + BASE_SEGMENT_LENGTH - 1 >= l->width)
                    continue;
                drawRect(l->m_pSurfaces, col, row, col + BASE_SEGMENT_LENGTH,
                    row + BASE_SEGMENT_LENGTH, l->width, GameObject(WALL_MARIO, -1));
                fillRect(l->m_pSurfaces, col + 1, row + 1, col + BASE_SEGMENT_LENGTH - 1,
                    row + BASE_SEGMENT_LENGTH - 1, l->width, GameObject(FLOOR_STONE, -1));
                fillRectPrb(l->m_pEntities, col + 1, row + 1, col + BASE_SEGMENT_LENGTH - 1,
                    row + BASE_SEGMENT_LENGTH - 1, l->width, GameObject(MONST_SNEK, 0x7FFFFFFF),
                    &rng, std::numeric_limits<unsigned int>::max() >> 1);
            }
        }
        l->m_pEntities[l->width + 1] = GameObject();
    }

    static void GenerateSpiderLevel(Level* l)
    {
        std::random_device rd;
        std::mt19937 rng = std::mt19937(rd());
        unsigned int segments = BASE_SEGMENTS_PER_LEVEL;
        if (INCR_SEGMENTS_AFTER_COUNT != 0)
            segments += l->difficulty / INCR_SEGMENTS_AFTER_COUNT;
        l->width = segments * BASE_SEGMENT_LENGTH;
        l->height = segments * BASE_SEGMENT_LENGTH;
        l->m_pEntities = new GameObject[l->width * l->height];
        l->m_pFurnishings = new GameObject[l->width * l->height];
        l->m_pSurfaces = new GameObject[l->width * l->height];
        l->m_pOverlays = new GameObject[l->width * l->height];
        for (unsigned int row = 0; row < l->height; row += BASE_SEGMENT_LENGTH)
        {
            for (unsigned int col = 0; col < l->width; col += BASE_SEGMENT_LENGTH)
            {
                drawRect(l->m_pSurfaces, col, row, col + BASE_SEGMENT_LENGTH,
                    row + BASE_SEGMENT_LENGTH, l->width, GameObject(WALL_MARIO, -1));
                fillRect(l->m_pSurfaces, col + 1, row + 1, col + BASE_SEGMENT_LENGTH - 1,
                    row + BASE_SEGMENT_LENGTH - 1, l->width, GameObject(FLOOR_DIRT, -1));
                unsigned int holePunchOffset = BASE_SEGMENT_LENGTH / 2;
                l->m_pSurfaces[(row + holePunchOffset) * l->width + col].setCode(FLOOR_DIRT);
                l->m_pSurfaces[(row + holePunchOffset) * l->width + col + BASE_SEGMENT_LENGTH - 1].setCode(FLOOR_DIRT);
                l->m_pSurfaces[row * l->width + col + holePunchOffset].setCode(FLOOR_DIRT);
                l->m_pSurfaces[(row + BASE_SEGMENT_LENGTH - 1) * l->width + col + holePunchOffset].setCode(FLOOR_DIRT);

                l->m_pEntities[(row + holePunchOffset) * l->width + col + holePunchOffset] = GameObject(SPAWN_SPIDER, 1);
            }
        }
        drawRect(l->m_pSurfaces, 0, 0, l->width, l->height, l->width, GameObject(WALL_BRICK, -1));
        unsigned int row, col;
        ObjectCode surf;
        do
        {
            row = rng() % l->height;
            col = rng() % l->width;
            surf = l->m_pSurfaces[row * l->width + col].getCode();
        } while (l->m_pEntities[row * l->width + col].getCode() != ObjectCode::NONE ||
            (surf >= MIN_WALL && surf <= MAX_WALL));
        l->m_pFurnishings[row * l->width + col] = GameObject(STAIRS, -1);
    }

    static void drawRect(GameObject* arr, unsigned int left, unsigned int top,
        unsigned int right, unsigned int bottom, unsigned int width, GameObject& templ)
    {
        // Go top, bottom, left, right. On sides, remember that corners are done.
        fillRect(arr, left, top, right, top + 1, width, templ);
        fillRect(arr, left, bottom - 1, right, bottom, width, templ);
        fillRect(arr, left, top + 1, left + 1, bottom - 1, width, templ);
        fillRect(arr, right - 1, top + 1, right, bottom - 1, width, templ);
    }

    static void fillRect(GameObject* arr, unsigned int left, unsigned int top,
        unsigned int right, unsigned int bottom, unsigned int width, GameObject& templ)
    {
        for (unsigned int r = top; r < bottom; r++)
        {
            for (unsigned int c = left; c < right; c++)
            {
                arr[r * width + c] = GameObject(templ);
            }
        }
    }

    static void fillRectPrb(GameObject* arr, unsigned int left, unsigned int top,
        unsigned int right, unsigned int bottom, unsigned int width, GameObject templ,
        std::mt19937* rng, unsigned int prbThreshLTCreate)
    {
        for (unsigned int r = top; r < bottom; r++)
        {
            for (unsigned int c = left; c < right; c++)
            {
                if (rng->operator()() < prbThreshLTCreate)
                    arr[r * width + c] = GameObject(templ);
            }
        }
    }
};