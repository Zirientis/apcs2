#pragma once
#include "GameObject.h"
#include "Position.h"

#include <random>
class Level
{
    friend class Game;
private:
    GameObject* m_pOverlays;
    GameObject* m_pEntities;
    GameObject* m_pFurnishings;
    GameObject* m_pSurfaces;

    int difficulty;
    unsigned int width, height;

    static const unsigned int BASE_SEGMENTS_PER_LEVEL = 3;
    static const unsigned int INCR_SEGMENTS_AFTER_COUNT = 2;
    static const unsigned int BASE_SEGMENT_LENGTH = 11;
    void drawRect(GameObject*, unsigned int, unsigned int,
        unsigned int, unsigned int, GameObject&);
    void fillRect(GameObject*, unsigned int, unsigned int,
        unsigned int, unsigned int, GameObject&);
public:
    Level(int, std::mt19937);
    ~Level();
    const GameObject* GetOverlayAt(Position);
    const GameObject* GetEntityAt(Position);
    const GameObject* GetFurnishingAt(Position);
    const GameObject* GetSurfaceAt(Position);
    unsigned int GetWidth();
    unsigned int GetHeight();
};
