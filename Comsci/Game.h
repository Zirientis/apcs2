#pragma once
#include "Level.h"
#include "Position.h"
#include "ActionCode.h"
#include <Windows.h>

#include <vector>
#include <random>

#define INPUT_HANDLE_NAME L"Game Input Ready"
#define TEXT_HANDLE_NAME L"Text Line Displayed"

#ifdef _DEBUG
#define WIZARD_MODE true
#else
#define WIZARD_MODE false
#endif

class Game
{
private:
    bool ready;
    static const int PLAYER_DAMAGE = 30;
    Level* m_pCurrentLevel;
    Position* m_pPlayerPositions; // For Game bookkeeping
    int m_numPlayers;
    int m_activePlayer;
    bool playersPlaced;
    Position synchronizedPos;
    const wchar_t* synchronizedTextString;
    HANDLE inputEvent; // setting this one means input is ready.
    HANDLE textEvent; // if unsignalled, there is text to show and then signal
    int64_t score;
    void(*getInput) (void*, Position*, const wchar_t*);
    LevelType gameType;
    std::mt19937 random;

    ActionCode moveEntity(Position start, Position end);
    ActionCode placeEntity(GameObject& templateObj, Position pos, bool force);
    void advanceLevel();
    void showText(const wchar_t* textString);
public:
    Game(int, void (*getInput) (void*, Position*, const wchar_t*));
    ~Game();
    void start();

    bool IsReady();

    int getActivePlayer();
    //std::vector<Action> getPotentialPlayerActions();
    void DefaultMemberGetInput(Position*, const wchar_t*); // game thread only!
    bool MaybeSendPosition(Position); // Returns whether the position was set
    const wchar_t* GetOutputText();
    const int GetDifficulty();
    const int64_t GetScore();

    const GameObject* GetEntityAt(Position);
    const GameObject* GetFurnishingAt(Position);
    const GameObject* GetSurfaceAt(Position);
    const GameObject* GetOverlayAt(Position);
    unsigned int GetWidth();
    unsigned int GetHeight();
};

void DefaultInputFunc(void*, Position*, const wchar_t*); // Game thread only!
DWORD WINAPI GameThreadEntryProc(void*);

inline void AssertPositionChangeValid(Position start, Position end)
{
    const int xDiff = (int)start.xTile - (int)end.xTile;
    if (!(xDiff == 1 || xDiff == 0 || xDiff == -1))
        __debugbreak();
    const int yDiff = (int)start.yTile - (int)end.yTile;
    if (!(yDiff == 1 || yDiff == 0 || yDiff == -1))
        __debugbreak();
}