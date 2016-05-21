#pragma once

enum ObjectCode {
    WALL_MARIO,
    WALL_BRICK,

    MAX_WALL = WALL_BRICK,

    FLOOR_DIRT,
    FLOOR_WEIRD,
    FLOOR_STONE,

    MAX_FLOOR = FLOOR_STONE,

    STAIRS,

    INDICATOR_GREEN,
    INDICATOR_RED,
    INDICATOR_BLUE,
    INDICATOR_GRAY,
    INDICATOR_NO,

    MAX_INDICATOR = INDICATOR_NO,

    TRAP_SPIKE_VIS,

    MAX_TRAP = TRAP_SPIKE_VIS,

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
    U_16,
    U_17,
    U_18,
    U_19,
    U_20,
    U_21,

    MONST_ZOMBIE,


    PLAYER_1,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4,
    MAX_PLAYER = PLAYER_4,

    MONST_SPIDER, // THESE NEED TO GO TOGETHER
    MONST_OOZE,
    MAX_MONST = MONST_OOZE,

    NONE,
    MAX
};