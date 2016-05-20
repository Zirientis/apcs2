#include "Game.h"

#include "GameObject.h"
#include "ObjectCode.h"
#include "Position.h"

#include <Windows.h>

#include <vector>
Game::Game(int numPlayers, void (*getInputFunc) (void*, Position*))
{
    m_pCurrentLevel = nullptr;
    m_numPlayers = numPlayers;
    m_pPlayers = new GameObject[numPlayers];
    m_pPlayerPositions = new Position[numPlayers];
    for (int i = 0; i < m_numPlayers; i++)
    {
        m_pPlayerPositions[i] = { 0, 0 };
    }
    m_activePlayer = 0;
    getInput = getInputFunc;
    synchronizedPos.xTile = 0;
    synchronizedPos.yTile = 0;
    inputEvent = CreateEvent(NULL, true, false, LPCWSTR(u"Game Input Ready"));
}

Game::~Game()
{
    delete m_pCurrentLevel;
    m_pCurrentLevel = nullptr;

    delete[] m_pPlayers;
    m_pPlayers = nullptr;
    delete[] m_pPlayerPositions;
    m_pPlayerPositions = nullptr;
    getInput = nullptr;
    synchronizedPos.xTile = 0;
    synchronizedPos.yTile = 0;
    CloseHandle(inputEvent);
    inputEvent = nullptr;
}

void Game::start()
{
    advanceLevel();
    for (;;) // forever
    {
        for (m_activePlayer = 0; m_activePlayer < m_numPlayers; m_activePlayer++)
        {
            // Set the necessary overlays
            Position playerPos = m_pPlayerPositions[m_activePlayer];
            unsigned int ptrOffset = playerPos.yTile * m_pCurrentLevel->GetWidth() + playerPos.xTile;
            GameObject* overlay = m_pCurrentLevel->m_pOverlays + ptrOffset;
            overlay->setCode(ObjectCode::INDICATOR_BLUE);
            // Wait for input
            Position p;
            do
            {
                getInput(this, &p);
            } while (p.xTile >= m_pCurrentLevel->GetWidth() || p.yTile >= m_pCurrentLevel->GetHeight());
            // deactivate the overlay
            overlay->setCode(ObjectCode::NONE);
            // Currently assuming the player can fly to any location in 1 turn.
            // Only move the player if the surface allows it and there is no other entity already there.
            // 1. Check to ensure no other entity is present.
            // 2. Call the surface's onBeforeWalk().
            // 3. Call the current furnishing's onAfterWalk().
            // 4. Move the player's GameObject and update m_pPlayerPositions.
            // 5. Call the furnishing's onWalk().
            if (m_pCurrentLevel->GetEntityAt(p)->getCode() == ObjectCode::NONE) // tile is empty
            {
                Position currentPlayerPos = m_pPlayerPositions[m_activePlayer];
                GameObject& currentPlayer = m_pPlayers[m_activePlayer];
                GameObject* surf = m_pCurrentLevel->m_pSurfaces + (p.yTile * m_pCurrentLevel->GetWidth() + p.xTile);
                GameObject* oldFurn = m_pCurrentLevel->m_pFurnishings + (currentPlayerPos.yTile * m_pCurrentLevel->GetWidth() + currentPlayerPos.xTile);

                if (surf->onBeforeWalk(currentPlayer) && oldFurn->onAfterWalk(currentPlayer))
                // ok to move to new tile, and old tile allows it (after effects from furnishing run)
                {
                    GameObject* ent = m_pCurrentLevel->m_pEntities + (p.yTile * m_pCurrentLevel->GetWidth() + p.xTile);
                    *ent = currentPlayer;
                    m_pPlayerPositions[m_activePlayer] = p;
                    currentPlayerPos = p;
                    GameObject* newFurn = m_pCurrentLevel->m_pFurnishings + (p.yTile * m_pCurrentLevel->GetWidth() + p.xTile);
                    newFurn->onWalk(currentPlayer);
                }
            }
        }
        // AI logic runs here
    }
}

void Game::advanceLevel()
{
    int oldDiff = 0;
    if (m_pCurrentLevel)
    {
        oldDiff = m_pCurrentLevel->difficulty;
        delete m_pCurrentLevel;
    }
    m_pCurrentLevel = new Level(oldDiff + 1);
}

int Game::getActivePlayer() // main thread, and only when locked
{
    return m_activePlayer;
}
/*
std::vector<Action> Game::getPotentialPlayerActions()
{

}
*/
bool Game::MaybeSendPosition(Position pos) // main thread only!
{
    // First check to ensure the game thread is done using the position.
    // It should be nonsignalled. If signaled, game thread is still active.
    if (WaitForSingleObject(inputEvent, 0) != WAIT_TIMEOUT)
    {
        //DebugBreak();
        return false;
    }
    synchronizedPos.xTile = pos.xTile;
    synchronizedPos.yTile = pos.yTile;
    SetEvent(inputEvent);
    return true;
}

void Game::DefaultMemberGetInput(Position* outPos) // game thread only!
{
    WaitForSingleObject(inputEvent, INFINITE);
    outPos->xTile = synchronizedPos.xTile;
    outPos->yTile = synchronizedPos.yTile;
    ResetEvent(inputEvent);
}

const GameObject* Game::GetEntityAt(Position p)
{
    return m_pCurrentLevel->GetEntityAt(p);
}

const GameObject* Game::GetFurnishingAt(Position p)
{
    return m_pCurrentLevel->GetFurnishingAt(p);
}

const GameObject* Game::GetSurfaceAt(Position p)
{
    return m_pCurrentLevel->GetSurfaceAt(p);
}

const GameObject* Game::GetOverlayAt(Position p)
{
    return m_pCurrentLevel->GetOverlayAt(p);
}

unsigned int Game::GetWidth()
{
    return m_pCurrentLevel->GetWidth();
}

unsigned int Game::GetHeight()
{
    return m_pCurrentLevel->GetHeight();
}

void DefaultInputFunc(void* pGameVoid, Position* outPos) // game thread only!
{
    Game* game = reinterpret_cast<Game*>(pGameVoid);
    game->DefaultMemberGetInput(outPos);
}

DWORD WINAPI GameThreadEntryProc(void* pGameVoid)
{
    Game* game = reinterpret_cast<Game*>(pGameVoid);
    game->start();
    return 0;
}
