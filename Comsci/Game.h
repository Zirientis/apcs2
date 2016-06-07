#pragma once
#include "GameType.h"
#include "Position.h"
#include "GameObject.h"
class Game
{
public:
    virtual void start() = 0;

    virtual bool IsReady() = 0;

    virtual int getActivePlayer() = 0;
    //std::vector<Action> getPotentialPlayerActions();
    virtual void DefaultMemberGetInput(Position*, const wchar_t*) = 0; // game thread only!
    virtual bool MaybeSendPosition(Position) = 0; // Returns whether the position was set
    virtual const wchar_t* GetOutputText() = 0;
    virtual const int GetDifficulty() = 0;
    virtual const int64_t GetScore() = 0;

    virtual const GameObject* GetEntityAt(Position) = 0;
    virtual const GameObject* GetFurnishingAt(Position) = 0;
    virtual const GameObject* GetSurfaceAt(Position) = 0;
    virtual const GameObject* GetOverlayAt(Position) = 0;
    virtual unsigned int GetWidth() = 0;
    virtual unsigned int GetHeight() = 0;
};
