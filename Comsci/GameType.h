#pragma once
enum GameType
{
    GT_SPIDER,
    GT_SNEK,
    GT_CLASSIC,
    GT_MAX
};

inline wchar_t* GetGameTypeStringW(GameType t)
{
    switch (t)
    {
    case GT_SPIDER:
        return L"Spider Onslaught!";
    case GT_SNEK:
        return L"SNEK";
    case GT_CLASSIC:
        return L"Classic";
    default:
        __debugbreak();
        return nullptr;
    }
}

inline unsigned int GetGameTypeMinPlayer(GameType t)
{
    if (t >= GameType::GT_MAX)
    {
        __debugbreak();
        return -1;
    }
    else
        return 1;
}

inline unsigned int GetGameTypeMaxPlayer(GameType t)
{
    switch (t)
    {
    case GT_SPIDER:
        return 3;
    case GT_SNEK:
        return 1;
    case GT_CLASSIC:
        return 1;
    default:
        __debugbreak();
        return -1;
    }
}