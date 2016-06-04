#pragma once
#include "Level.h"
#include "Position.h"
#include "ActionCode.h"
#include "GameType.h"
#include <Windows.h>

#include <vector>
#include <random>
#include <string>

#define INPUT_HANDLE_NAME L"Game Input Ready"
#define TEXT_HANDLE_NAME L"Text Line Displayed"

#ifdef _DEBUG
#define WIZARD_MODE true
#ifndef DEBUG_SPAWNER
#define DEBUG_SPAWNER true
#endif // DEBUG_SPAWNER
#else
#define WIZARD_MODE false
#ifdef DEBUG_SPAWNER
#undef DEBUG_SPAWNER
#endif // DEBUG_SPAWNER
#define DEBUG_SPAWNER false
#endif // _DEBUG

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
    GameType gameType;
    std::mt19937 random;

    ActionCode moveEntity(Position start, Position end);
    ActionCode placeEntity(GameObject& templateObj, Position pos, bool force);
    void advanceLevel();
    void showText(const wchar_t* textString);
    void doStairAction(ObjectCode);
public:
    Game(int, GameType, void (*getInput) (void*, Position*, const wchar_t*));
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

inline void Game::doStairAction(ObjectCode triggerCode)
{
    switch (gameType)
    {
    case GT_SPIDER:
        if (IsCodePlayer(triggerCode))
        {
            //return AC_STAIR_TRIGGERED;
            showText(L"The stairs collapse downward!");
            showText(L"It seems that your adventure is over!");
            {
                // HACKETY HACK HACK
                std::wstring outstr;
                outstr += L"Game Over! Your score was ";
                outstr += std::to_wstring(score);
                MessageBox(NULL, outstr.data(), L"Comsci", 0);
                //delete this;
                ExitThread(0);
            }
        }
        break;
    case GT_CLASSIC:
        // here we go
        __debugbreak();
        break;
    default:
        break;
    }
}