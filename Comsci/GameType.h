#pragma once
enum GameType
{
    GT_SPIDER,
    GT_SNEK,
    GT_CLASSIC
};

inline wchar_t* GetGameTypeStringW(GameType l)
{
    switch (l)
    {
    case GT_SPIDER:
        return L"Spider Onslaught!";
    case GT_SNEK:
        return L"SNEK";
    case GT_CLASSIC:
        return L"Classic";
    }
}
