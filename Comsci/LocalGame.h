#pragma once
#include "Level.h"
#include "LevelGenerator.h"
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
#ifndef NO_DEBUG_SPAWNER
#define DEBUG_SPAWNER true
#else
#define DEBUG_SPAWNER false
#endif // DEBUG_SPAWNER
#ifndef NO_DEBUG_MOVE
#define DEBUG_MOVE true
#else
#define DEBUG_MOVE false
#endif // DEBUG_MOVE
#else
#define WIZARD_MODE false
#ifdef DEBUG_SPAWNER
#undef DEBUG_SPAWNER
#endif // DEBUG_SPAWNER
#define DEBUG_SPAWNER false
#ifdef DEBUG_MOVE
#undef DEBUG_MOVE
#endif // DEBUG_MOVE
#define DEBUG_MOVE false
#endif // _DEBUG

class LocalGame : public Game
{
public:
    LocalGame(int, GameType, void(*getInput) (void*, Position*, const wchar_t*));
    virtual ~LocalGame();
    virtual void start();

    virtual bool IsReady();

    virtual int getActivePlayer();
    //std::vector<Action> getPotentialPlayerActions();
    virtual void DefaultMemberGetInput(Position*, const wchar_t*); // game thread only!
    virtual bool MaybeSendPosition(Position); // Returns whether the position was set
    virtual const wchar_t* GetOutputText();
    virtual const int GetDifficulty();
    virtual const int64_t GetScore();

    virtual const GameObject* GetEntityAt(Position);
    virtual const GameObject* GetFurnishingAt(Position);
    virtual const GameObject* GetSurfaceAt(Position);
    virtual const GameObject* GetOverlayAt(Position);
    virtual unsigned int GetWidth();
    virtual unsigned int GetHeight();
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
    void showBanner();
    void advanceLevel();
    void showText(const wchar_t* textString);
    void DEATH();
    void REAPER(int hp);
    bool doStairAction(ObjectCode); // true if invalidation req
    void tick(uint64_t);
};

void DefaultInputFunc(void*, Position*, const wchar_t*); // Game thread only!
DWORD WINAPI GameThreadEntryProc(void*);
