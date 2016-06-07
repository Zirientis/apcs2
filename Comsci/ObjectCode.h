#pragma once
#include <random>
enum ObjectCode {
    MIN_WALL,
    WALL_MARIO = MIN_WALL,
    WALL_BRICK,
    MAX_WALL = WALL_BRICK,

    MIN_FLOOR,
    FLOOR_DIRT = MIN_FLOOR,
    FLOOR_WEIRD,
    FLOOR_STONE,
    MAX_FLOOR = FLOOR_STONE,

    STAIRS,

    MIN_INDICATOR,
    INDICATOR_GREEN = MIN_INDICATOR,
    INDICATOR_RED,
    INDICATOR_BLUE,
    INDICATOR_GRAY,
    MAX_INDICATOR = INDICATOR_GRAY,

    MIN_FOG,
    FOG_A = MIN_FOG,
    FOG_B,
    MAX_FOG = FOG_B,

    MIN_TRAP,
    TRAP_SPIKE_VIS = MIN_TRAP,
    TRAP_TARPIT,
    TRAP_PUDDLE,
    MAX_TRAP = TRAP_PUDDLE,

    MIN_SPAWN,
    SPAWN_EYEBALL = MIN_SPAWN,
    SPAWN_SPIDER,
    SPAWN_OOZE,
    SPAWN_SNEK, // [sic]
    MAX_SPAWN = SPAWN_SNEK,


    MIN_PLAYER,
    PLAYER_1 = MIN_PLAYER,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4,
    MAX_PLAYER = PLAYER_4,

    COIN,

    MIN_POTION,
    POTION_RED = MIN_POTION,
    POTION_PURPLE,
    POTION_YELLOW,
    MAX_POTION = POTION_YELLOW,

    MIN_WEAPON,
    WEAPON_SWORD = MIN_WEAPON,
    WEAPON_SWORD_BLUE_GLOW,
    WEAPON_SWORD_YELLOW_GLOW,
    WEAPON_FLAIL,
    MAX_WEAPON = WEAPON_FLAIL,

    MIN_PROTECTIVE,
    PROTECTIVE_SHIELD_BASIC = MIN_PROTECTIVE,
    MAX_PROTECTIVE = PROTECTIVE_SHIELD_BASIC,
    u1,
    u2,
    u3,

    MIN_MONST,
    MONST_PLAYER_GHOST = MIN_MONST,
    MIN_RAND_MONST,
    MONST_SNEK = MIN_RAND_MONST,
    MONST_EYEBALL,
    MONST_SPIDER,
    MONST_OOZE,
    MAX_MONST = MONST_OOZE,

    NONE,
    MAX
};


inline bool IsCodeNone(ObjectCode query)
{
    return query == NONE;
}

inline bool IsCodeWall(ObjectCode query)
{
    return query >= MIN_WALL && query <= MAX_WALL;
}

inline bool IsCodeFloor(ObjectCode query)
{
    return query >= MIN_FLOOR && query <= MAX_FLOOR;
}

inline bool IsCodePotion(ObjectCode query)
{
    return query >= MIN_POTION && query <= MAX_POTION;
}

inline bool IsCodeCoin(ObjectCode query)
{
    return query == COIN;
}

inline bool IsCodeMonst(ObjectCode query)
{
    return query >= MIN_MONST && query <= MAX_MONST;
}

inline bool IsCodeSpawner(ObjectCode query)
{
    return query >= MIN_SPAWN && query <= MAX_SPAWN;
}

inline bool IsCodePlayer(ObjectCode query)
{
    return query >= MIN_PLAYER && query <= MAX_PLAYER;
}

inline ObjectCode GetSpawnedItem(ObjectCode spawner)
{
    switch (spawner)
    {
    case SPAWN_SNEK:
        return MONST_SNEK;
    case SPAWN_SPIDER:
        return MONST_SPIDER;
    case SPAWN_EYEBALL:
        return MONST_EYEBALL;
    case SPAWN_OOZE:
        return MONST_OOZE;
    case MONST_PLAYER_GHOST: // fall through
    default:
        return NONE;
    }
}

inline ObjectCode GetSpawner(ObjectCode spawnee)
{
    switch (spawnee)
    {
    case MONST_SNEK:
        return SPAWN_SNEK;
    case MONST_EYEBALL:
        return SPAWN_EYEBALL;
    case MONST_SPIDER:
        return SPAWN_SPIDER;
    case MONST_OOZE:
        return SPAWN_OOZE;
    default:
        return NONE;
    }
}

inline int GetScoreChange(ObjectCode query)
{
    if (IsCodePotion(query))
        return 100;
    if (IsCodeCoin(query))
        return 50;
    if (IsCodeMonst(query))
        return 25;
    return 0;
}

inline int GetDefaultHealth(ObjectCode query)
{
    if (IsCodeSpawner(query))
        return 1;
    if (IsCodeCoin(query) || IsCodePotion(query))
        return 1;
    if (IsCodePlayer(query))
        return 100;
    if (IsCodeMonst(query))
        switch (query)
        {
        case MONST_SNEK:
            return 10;
        case MONST_SPIDER:
            return 25;
        case MONST_PLAYER_GHOST:
            return 100;
        case MONST_EYEBALL:
            return 50;
        case MONST_OOZE:
            return 75;
        default:
            return 50;
        }
    return -1;
}

inline ObjectCode GetRandomWallCode(std::mt19937* rng)
{
    unsigned int numOfWalls = MAX_WALL - MIN_WALL + 1;
    return static_cast<ObjectCode>(MIN_WALL + (rng->operator()() % numOfWalls));
}

inline ObjectCode GetRandomFloorCode(std::mt19937* rng)
{
    unsigned int numOfFloors = MAX_FLOOR - MIN_FLOOR + 1;
    return static_cast<ObjectCode>(MIN_FLOOR + (rng->operator()() % numOfFloors));
}

inline ObjectCode GetRandomMonstCode(std::mt19937* rng)
{
    unsigned int numOfMonst = MAX_MONST - MIN_RAND_MONST + 1;
    return static_cast<ObjectCode>(MIN_RAND_MONST + (rng->operator()() % numOfMonst));
}

inline ObjectCode GetRandomSpawnerCode(std::mt19937* rng)
{
    unsigned int numOfSpawner = MAX_SPAWN - MIN_SPAWN + 1;
    return static_cast<ObjectCode>(MIN_SPAWN + (rng->operator()() % numOfSpawner));
}
