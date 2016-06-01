#pragma once
#include "GameType.h"
enum LevelType
{
    LT_SPIDER,
    LT_SNEK,
    LT_CLASSIC_1,
    LT_MAX
};

inline LevelType LevelTypeFromGameType(GameType t)
{
    switch (t)
    {
    case GT_SPIDER:
        return LT_SPIDER;
    case GT_SNEK:
        return LT_SNEK;
    case GT_CLASSIC:
        return LT_CLASSIC_1;
    default:
        __debugbreak();
        return LT_MAX;
    }
}
