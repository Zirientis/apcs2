#pragma once
#include "Level.h"
#include "Position.h"
#include <Windows.h>

#include <vector>

#define INPUT_HANDLE_NAME L"Game Input Ready"
#define TEXT_HANDLE_NAME L"Text Line Displayed"
class Game
{
private:
    Level* m_pCurrentLevel;
    GameObject* m_pPlayers;
    Position* m_pPlayerPositions; // For Game bookkeeping
    int m_numPlayers;
    int m_activePlayer;
    bool playersPlaced;
    Position synchronizedPos;
    const wchar_t* synchronizedTextString;
    HANDLE inputEvent; // setting this one means input is ready.
    HANDLE textEvent; // if unsignalled, there is text to show and then signal
    void(*getInput) (void*, Position*, const wchar_t*);

    bool moveEntity(Position start, Position end);
    bool placeEntity(GameObject& templateObj, Position pos, bool force);
    void advanceLevel();
    void showText(const wchar_t* textString);
public:
    Game(int, void (*getInput) (void*, Position*, const wchar_t*));
    ~Game();
    void start();

    int getActivePlayer();
    //std::vector<Action> getPotentialPlayerActions();
    void DefaultMemberGetInput(Position*, const wchar_t*); // game thread only!
    bool MaybeSendPosition(Position); // Returns whether the position was set
    const wchar_t* GetOutputText();

    const GameObject* GetEntityAt(Position);
    const GameObject* GetFurnishingAt(Position);
    const GameObject* GetSurfaceAt(Position);
    const GameObject* GetOverlayAt(Position);
    unsigned int GetWidth();
    unsigned int GetHeight();
};

void DefaultInputFunc(void*, Position*, const wchar_t*); // Game thread only!
DWORD WINAPI GameThreadEntryProc(void*);
