#pragma once
#include "GameObject.h"
#include "Position.h"
#include "LevelType.h"

#ifndef BASE_SEGMENTS_PER_LEVEL
#define BASE_SEGMENTS_PER_LEVEL 3
#endif // #ifndef BASE_SEGMENTS_PER_LEVEL

#ifndef INCR_SEGMENTS_AFTER_COUNT
#define INCR_SEGMENTS_AFTER_COUNT 2
#endif // #ifndef INCR_SEGMENTS_AFTER_COUNT

#ifndef BASE_SEGMENT_LENGTH
#define BASE_SEGMENT_LENGTH 11
#endif // #ifndef BASE_SEGMENT_LENGTH

class Level
{
    friend class Game;
    friend class LevelGenerator;
private:
    GameObject* m_pOverlays;
    GameObject* m_pEntities;
    GameObject* m_pFurnishings;
    GameObject* m_pSurfaces;

    int difficulty;
    unsigned int width, height;

    void MarkAllEntitiesReady();
public:
    Level(int, LevelType, const unsigned int);
    ~Level();
    const GameObject* GetOverlayAt(Position);
    const GameObject* GetEntityAt(Position);
    const GameObject* GetFurnishingAt(Position);
    const GameObject* GetSurfaceAt(Position);
    unsigned int GetWidth();
    unsigned int GetHeight();
};
