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
    SPAWN_ZOMBIE = MIN_SPAWN,
    SPAWN_SPIDER,
    SPAWN_OOZE,
    MAX_SPAWN = SPAWN_OOZE,

    U_1,
    U_2,
    U_3,
    U_4,
    U_5,
    U_6,
    U_7,
    U_8,
    U_9,
    U_10,
    U_11,
    U_12,
    U_13,
    U_14,
    U_15,


    MIN_PLAYER,
    PLAYER_1 = MIN_PLAYER,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4,
    MAX_PLAYER = PLAYER_4,

    MIN_MONST,
    MONST_ZOMBIE = MIN_MONST,
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
    case SPAWN_ZOMBIE:
        return MONST_ZOMBIE;
    case SPAWN_SPIDER:
        return MONST_SPIDER;
    case SPAWN_OOZE:
        return MONST_OOZE;
    default:
        return NONE;
    }
}

inline ObjectCode GetSpawner(ObjectCode spawnee)
{
    switch (spawnee)
    {
    case MONST_ZOMBIE:
        return SPAWN_ZOMBIE;
    case MONST_SPIDER:
        return SPAWN_SPIDER;
    case MONST_OOZE:
        return SPAWN_OOZE;
    }
}