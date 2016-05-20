#pragma once

enum ObjectCode {
    WALL_MARIO,
    WALL_BRICK,

    MAX_WALL = WALL_BRICK,

    FLOOR_DIRT,
    FLOOR_WEIRD,
    FLOOR_CRACKED,

    MAX_FLOOR = FLOOR_CRACKED,

    INDICATOR_GREEN,
    INDICATOR_RED,
    INDICATOR_BLUE,
    INDICATOR_NO,

    MAX_INDICATOR = INDICATOR_NO,

    U_1,
    U_2,
    U_3,
    U_4,
    U_5,
    U_6,
    U_7,
    U_8,
    U_9,


    MONST_SPIDER,

    MAX_MONST = MONST_SPIDER,

    PLAYER,

    NONE,
    MAX
};