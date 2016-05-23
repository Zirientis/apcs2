#pragma once
struct Position
{
    unsigned int xTile;
    unsigned int yTile;

    inline bool operator==(Position& other)
    {
        return xTile == other.xTile && yTile == other.yTile;
    }

    inline bool operator!=(Position& other)
    {
        return !(xTile == other.xTile && yTile == other.yTile);
    }
};
