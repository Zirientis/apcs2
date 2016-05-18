#pragma once

enum ObjectCode {
    WALL_MARIO,
    WALL_BRICK,

    MAX_WALL = WALL_BRICK,

    FLOOR_DIRT,
    FLOOR_WEIRD,

    MAX_FLOOR = FLOOR_WEIRD,

    INDICATOR_GREEN,
    INDICATOR_RED,

    MAX_INDICATOR = INDICATOR_RED,

    MONST_SPIDER,

    MAX_MONST = MONST_SPIDER,

    PLAYER,

    MAX
};