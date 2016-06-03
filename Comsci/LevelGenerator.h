#pragma once
#include "LevelType.h"
#include <random>
#include <limits>
class LevelGenerator
{
public:
    static void GenerateLevel(Level* l, LevelType type, const unsigned int seed)
    {
        switch (type)
        {
        case LT_SPIDER:
            GenerateSpiderLevel(l, seed);
            return;
        case LT_SNEK:
            GenerateSnekLevel(l, seed);
            return;
        case LT_CLASSIC_1:
            GenerateClassic1Level(l, seed);
            return;
        default:
            __debugbreak();
            // Unknown level type!
        }
    }

    static void GenerateClassic1Level(Level* l, const unsigned int seed)
    {
        std::mt19937 rng = std::mt19937(seed);
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
                GameObject wallTempl = GameObject(GetRandomWallCode(&rng));
                GameObject floorTempl = GameObject(GetRandomFloorCode(&rng));
                drawRect(l->m_pSurfaces, col, row, col + BASE_SEGMENT_LENGTH,
                    row + BASE_SEGMENT_LENGTH, l->width, wallTempl);
                fillRect(l->m_pSurfaces, col + 1, row + 1, col + BASE_SEGMENT_LENGTH - 1,
                    row + BASE_SEGMENT_LENGTH - 1, l->width, floorTempl);
                unsigned int holePunchOffset = BASE_SEGMENT_LENGTH / 2;
                if (col != 0)
                    l->m_pSurfaces[(row + holePunchOffset) * l->width + col] = floorTempl;
                if (col + BASE_SEGMENT_LENGTH != l->width)
                    l->m_pSurfaces[(row + holePunchOffset) * l->width + col + BASE_SEGMENT_LENGTH - 1] = floorTempl;
                if (row != 0)
                    l->m_pSurfaces[row * l->width + col + holePunchOffset] = floorTempl;
                if (row + BASE_SEGMENT_LENGTH != l->height)
                    l->m_pSurfaces[(row + BASE_SEGMENT_LENGTH - 1) * l->width + col + holePunchOffset] = floorTempl;
                // add some monsters (medium prb) and statues (low prb) and a chance of player 5
                fillRectPrbMonst(l->m_pEntities,
                    col + 1,
                    row + 1,
                    col + BASE_SEGMENT_LENGTH - 1,
                    row + BASE_SEGMENT_LENGTH - 1,
                    l->width,
                    &rng,
                    std::numeric_limits<unsigned int>::max() >> 5
                );
                fillRectPrbSpawner(l->m_pEntities,
                    col + 1,
                    row + 1,
                    col + BASE_SEGMENT_LENGTH - 1,
                    row + BASE_SEGMENT_LENGTH - 1,
                    l->width,
                    &rng,
                    std::numeric_limits<unsigned int>::max() >> 7
                );
            }
        }
        randPlaceOneInLevel(l, &rng, GameObject(STAIRS));
        if (rng() < std::numeric_limits<unsigned int>::max() >> 4)
            randPlaceOneInLevel(l, &rng, GameObject(MONST_PLAYER_GHOST));
    }

    static void GenerateSnekLevel(Level* l, const unsigned int seed)
    {
        std::mt19937 rng = std::mt19937(seed);
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
                    row + BASE_SEGMENT_LENGTH, l->width, GameObject(WALL_MARIO));
                fillRect(l->m_pSurfaces, col + 1, row + 1, col + BASE_SEGMENT_LENGTH - 1,
                    row + BASE_SEGMENT_LENGTH - 1, l->width, GameObject(FLOOR_STONE));
                fillRectPrb(l->m_pEntities, col + 1, row + 1, col + BASE_SEGMENT_LENGTH - 1,
                    row + BASE_SEGMENT_LENGTH - 1, l->width, GameObject(MONST_SNEK),
                    &rng, std::numeric_limits<unsigned int>::max() >> 2);
            }
        }
        l->m_pEntities[l->width + 1] = GameObject();
    }

    static void GenerateSpiderLevel(Level* l, const unsigned int seed)
    {
        std::mt19937 rng = std::mt19937(seed);
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
                    row + BASE_SEGMENT_LENGTH, l->width, GameObject(WALL_MARIO));
                fillRect(l->m_pSurfaces, col + 1, row + 1, col + BASE_SEGMENT_LENGTH - 1,
                    row + BASE_SEGMENT_LENGTH - 1, l->width, GameObject(FLOOR_DIRT));
                unsigned int holePunchOffset = BASE_SEGMENT_LENGTH / 2;
                l->m_pSurfaces[(row + holePunchOffset) * l->width + col].setCode(FLOOR_DIRT);
                l->m_pSurfaces[(row + holePunchOffset) * l->width + col + BASE_SEGMENT_LENGTH - 1].setCode(FLOOR_DIRT);
                l->m_pSurfaces[row * l->width + col + holePunchOffset].setCode(FLOOR_DIRT);
                l->m_pSurfaces[(row + BASE_SEGMENT_LENGTH - 1) * l->width + col + holePunchOffset].setCode(FLOOR_DIRT);

                l->m_pEntities[(row + holePunchOffset) * l->width + col + holePunchOffset] = GameObject(SPAWN_SPIDER);
            }
        }
        drawRect(l->m_pSurfaces, 0, 0, l->width, l->height, l->width, GameObject(WALL_BRICK));
        randPlaceOneInLevel(l, &rng, GameObject(STAIRS));
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
        unsigned int right, unsigned int bottom, unsigned int width, GameObject& templ,
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

    static void fillRectPrbMonst(GameObject* arr, unsigned int left, unsigned int top,
        unsigned int right, unsigned int bottom, unsigned int width,
        std::mt19937* rng, unsigned int prbThreshLTCreate)
    {
        for (unsigned int r = top; r < bottom; r++)
        {
            for (unsigned int c = left; c < right; c++)
            {
                if (rng->operator()() < prbThreshLTCreate)
                {
                    ObjectCode code = GetRandomMonstCode(rng);
                    arr[r * width + c] = GameObject(code);
                }
            }
        }
    }

    static void fillRectPrbSpawner(GameObject* arr, unsigned int left, unsigned int top,
        unsigned int right, unsigned int bottom, unsigned int width,
        std::mt19937* rng, unsigned int prbThreshLTCreate)
    {
        for (unsigned int r = top; r < bottom; r++)
        {
            for (unsigned int c = left; c < right; c++)
            {
                if (rng->operator()() < prbThreshLTCreate)
                {
                    ObjectCode code = GetRandomSpawnerCode(rng);
                    arr[r * width + c] = GameObject(code);
                }
            }
        }
    }

    static void randPlaceOneInLevel(Level* l, std::mt19937* rng, GameObject& templ)
    {
        // should refactor the following into a function
        unsigned int row, col;
        ObjectCode surf;
        do
        {
            row = rng->operator()() % l->height;
            col = rng->operator()() % l->width;
            surf = l->m_pSurfaces[row * l->width + col].getCode();
        } while (l->m_pEntities[row * l->width + col].getCode() != ObjectCode::NONE ||
            (surf >= MIN_WALL && surf <= MAX_WALL));
        l->m_pFurnishings[row * l->width + col] = GameObject(templ);
    }
};
