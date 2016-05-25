#pragma once

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
    SPAWN_SNAKE,
    MAX_SPAWN = SPAWN_SNAKE,


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
    WEAPON_FLAIL,
    MAX_WEAPON = WEAPON_FLAIL,

    u1,
    u2,
    u3,
    u4,
    u5,
    u6,

    MIN_MONST,
    MONST_PLAYER_GHOST = MIN_MONST,
    MONST_SNAKE,
    MONST_EYEBALL,
    MONST_SPIDER,
    MONST_OOZE,
    MAX_MONST = MONST_OOZE,

    NONE,
    MAX
};

inline ObjectCode GetSpawnedItem(ObjectCode spawner)
{
    switch (spawner)
    {
    case SPAWN_SNAKE:
        return MONST_SNAKE;
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
    case MONST_SNAKE:
        return SPAWN_SNAKE;
    case MONST_EYEBALL:
        return SPAWN_EYEBALL;
    case MONST_SPIDER:
        return SPAWN_SPIDER;
    case MONST_OOZE:
        return SPAWN_OOZE;
    }
}

inline int GetScoreChange(ObjectCode query)
{
    if (query >= MIN_POTION && query <= MAX_POTION)
        return 100;
    if (query >= MIN_MONST && query <= MAX_MONST)
        return 25;
    return 0;
}
