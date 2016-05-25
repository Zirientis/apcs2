#include "Game.h"

#include "GameObject.h"
#include "ObjectCode.h"
#include "Position.h"

#include <Windows.h>

#include <vector>
#include <random>
#include <cmath>
Game::Game(int numPlayers, void (*getInputFunc) (void*, Position*, const wchar_t*))
{
    std::random_device rd;
    random = std::mt19937(rd());
    m_pCurrentLevel = nullptr;
    m_numPlayers = numPlayers;
    m_pPlayerPositions = new Position[numPlayers];
    for (int i = 0; i < m_numPlayers; i++)
    {
        m_pPlayerPositions[i] = { 0, 0 };
    }
    m_activePlayer = 0;
    playersPlaced = false;
    synchronizedPos.xTile = 0;
    synchronizedPos.yTile = 0;
    synchronizedTextString = nullptr;
    inputEvent = CreateEvent(NULL, true, false, INPUT_HANDLE_NAME);
    textEvent = CreateEvent(NULL, true, true, TEXT_HANDLE_NAME);
    getInput = getInputFunc;
}

Game::~Game()
{
    delete m_pCurrentLevel;
    m_pCurrentLevel = nullptr;

    delete[] m_pPlayerPositions;
    m_pPlayerPositions = nullptr;
    getInput = nullptr;
    synchronizedPos.xTile = 0;
    synchronizedPos.yTile = 0;
    CloseHandle(inputEvent);
    inputEvent = nullptr;
    CloseHandle(textEvent);
    textEvent = nullptr;
}

void Game::start()
{
    advanceLevel();
    showText(L"Welcome to Comsci\u2122!\n");
    for (uint64_t turn = 0;;turn++) // forever
    {
        const unsigned int width = m_pCurrentLevel->GetWidth();
        const unsigned int height = m_pCurrentLevel->GetHeight();
        for (m_activePlayer = 0; m_activePlayer < m_numPlayers; m_activePlayer++)
        {
            // Set the necessary overlays
            Position playerPos = m_pPlayerPositions[m_activePlayer];
            unsigned int ptrOffset = playerPos.yTile * width + playerPos.xTile;
            GameObject* overlay = m_pCurrentLevel->m_pOverlays + ptrOffset;
            // Wait for input
            Position p;
            int xDiff, yDiff;
            do
            {
                if (!playersPlaced)
                    getInput(this, &p, L"Place your player...\n");
                else
                {
                    overlay->setCode(ObjectCode::INDICATOR_BLUE);
                    getInput(this, &p, L"Make your move...\n");
                }
                overlay->setCode(ObjectCode::NONE);
                xDiff = std::abs((int)p.xTile - (int)playerPos.xTile);
                yDiff = std::abs((int)p.yTile - (int)playerPos.yTile);
            } while (p.xTile >= width || p.yTile >= height ||
                (playersPlaced && (xDiff > 1 || yDiff > 1)));
            // deactivate the overlay

            ObjectCode targetEntCode = m_pCurrentLevel->m_pEntities[p.yTile * width + p.xTile].getCode();
            ObjectCode targetSurfCode = m_pCurrentLevel->m_pSurfaces[p.yTile * width + p.xTile].getCode();
            if (!playersPlaced)
            {
                GameObject playerTemplate = GameObject((ObjectCode)(ObjectCode::PLAYER_1 + m_activePlayer), 100);
                if (placeEntity(playerTemplate, p, false))
                {
                    m_pPlayerPositions[m_activePlayer] = p;
                }
                else
                {
                    showText(L"Try again!");
                    m_activePlayer--;
                }
            }
            else if (moveEntity(playerPos, p))
            {
                m_pPlayerPositions[m_activePlayer] = p;
            }
            else if (targetEntCode >= MIN_POTION && targetEntCode <= MAX_POTION)
            {
                showText(L"You quaff the potion.");
                m_pCurrentLevel->m_pEntities[p.yTile * width + p.xTile] = GameObject();
                if (!moveEntity(playerPos, p))
                {
                    // ASSERT: Couldn't move to square after taking potion!
                    DebugBreak();
                }
                else
                    m_pPlayerPositions[m_activePlayer] = p;
            }
            else if (p == playerPos)
            {
                showText(L"You look around curiously.");
            }
            else if (targetEntCode >= PLAYER_1 && targetEntCode <= MAX_PLAYER)
            {
                showText(L"You can't attack your teammate!");
            }
            else if (p != playerPos && targetSurfCode > MAX_WALL)
            {
                // attack
                showText(L"You attack the creature!");
                GameObject* npc = m_pCurrentLevel->m_pEntities + (p.yTile * width + p.xTile);
                if (npc->attack(PLAYER_DAMAGE) < 0)
                {
                    showText(L"You have mortally wounded it!");
                    *npc = GameObject();
                }
            }
            else
            {
                showText(L"Something was already there! You forfeit your turn.");
            }
        }
        playersPlaced = true;
        // AI logic runs here
        //MessageBox(NULL, LPCWSTR(u"AI Thinking..."), LPCWSTR(u"AI Thinking"), 0);
        showText(L"You hear the creatures of the dungeon begin to stir...");

        // Now allow each non-player entity to act
        // FIXME: GameObject should take a turnCreated, if objects should sleep for 1 turn.
        for (unsigned int i = 0; i < width * height; i++)
        {
            GameObject* npc = m_pCurrentLevel->m_pEntities + i;
            ObjectCode npcCode = npc->getCode();
            Position npcPos = Position{ i % width, i / width };
            if (npcCode == NONE || (npcCode >= MIN_PLAYER && npcCode <= MAX_PLAYER))
                continue;
            // Now do something!
            // On death, monster *should* have a chance to turn into a spawner.
            if (npc->getHealth() <= 0)
            {
                continue;
            }
            else if (npcCode >= MIN_MONST && npcCode <= MAX_MONST)
            {
                if (npcPos.xTile > 0 && npcPos.xTile < width - 1 && npcPos.yTile > 0 && npcPos.yTile < height - 1)
                {
                    Position movePos = Position{ npcPos.xTile + (random() % 3) - 1, npcPos.yTile + (random() % 3) - 1 };
                    AssertPositionChangeValid(npcPos, movePos);
                    if (!moveEntity(npcPos, movePos))
                    {
                        //
                    }
                }
            }
        }
        for (unsigned int i = 0; i < width * height; i++)
        {
            GameObject* npc = m_pCurrentLevel->m_pEntities + i;
            ObjectCode npcCode = npc->getCode();
            Position npcPos = Position{ i / width, i % width };
            if (npcCode == NONE || (npcCode >= MIN_PLAYER && npcCode <= MAX_PLAYER))
                continue;
            else if (npcCode >= MIN_SPAWN && npcCode <= MAX_SPAWN)
            {
                if (npcPos.xTile > 0 && npcPos.xTile < width - 1 && npcPos.yTile > 0 && npcPos.yTile < height - 1 && turn % 4 == 0)
                {
                    //npc->setCode(GetSpawnedItem(npcCode));
                    Position spawneePos = Position{ npcPos.xTile + (random() % 3) - 1, npcPos.yTile + (random() % 3) - 1 };
                    AssertPositionChangeValid(npcPos, spawneePos);
                    placeEntity(GameObject(GetSpawnedItem(npcCode), 25), spawneePos, false);
                }
            }
        }
    }
}

bool Game::moveEntity(Position start, Position end)
{
    // Currently assuming the player can fly to any location in 1 turn.
    // Only move the player if the surface allows it and there is no other entity already there.
    // 1. Check to ensure no other entity is present.
    // 2. Call the surface's onBeforeWalk().
    // 3. Call the current furnishing's onAfterWalk().
    // 4. Move the player's GameObject and update m_pPlayerPositions.
    // 5. Call the furnishing's onWalk().

    if (start == end)
    {
        return true; // Nothing happens!
    }

    if (m_pCurrentLevel->GetEntityAt(end)->getCode() == ObjectCode::NONE) // tile is empty
    {
        GameObject* oldEnt = m_pCurrentLevel->m_pEntities + (start.yTile * m_pCurrentLevel->GetWidth() + start.xTile);
        GameObject* newSurf = m_pCurrentLevel->m_pSurfaces + (end.yTile * m_pCurrentLevel->GetWidth() + end.xTile);
        GameObject* oldFurn = m_pCurrentLevel->m_pFurnishings + (start.yTile * m_pCurrentLevel->GetWidth() + start.xTile);

        if (newSurf->onBeforeWalk(oldEnt) && oldFurn->onAfterWalk(oldEnt))
        // ok to move to new tile, and old tile allows it (after effects from furnishing run)
        {
            GameObject* newEnt = m_pCurrentLevel->m_pEntities + (end.yTile * m_pCurrentLevel->GetWidth() + end.xTile);
            *newEnt = *oldEnt;
            *oldEnt = GameObject();
            GameObject* newFurn = m_pCurrentLevel->m_pFurnishings + (end.yTile * m_pCurrentLevel->GetWidth() + end.xTile);
            newFurn->onWalk(newEnt); // TODO: has issues if the trap moves the entity
            return true;
        }
    }
    return false;
}

bool Game::placeEntity(GameObject& templateObj, Position pos, bool force)
{
    GameObject* ent = m_pCurrentLevel->m_pEntities + (pos.yTile * m_pCurrentLevel->GetWidth() + pos.xTile);
    GameObject* surf = m_pCurrentLevel->m_pSurfaces + (pos.yTile * m_pCurrentLevel->GetWidth() + pos.xTile);
    if (force || (ent->getCode() == ObjectCode::NONE && surf->onBeforeWalk(nullptr)))
    {
        *ent = GameObject(templateObj);
        GameObject* furn = m_pCurrentLevel->m_pFurnishings + (pos.yTile * m_pCurrentLevel->GetWidth() + pos.xTile);
        furn->onWalk(ent);
        return true;
    }
    return false;
}

void Game::advanceLevel()
{
    int oldDiff = 0;
    if (m_pCurrentLevel)
    {
        oldDiff = m_pCurrentLevel->difficulty;
        delete m_pCurrentLevel;
    }
    m_pCurrentLevel = new Level(oldDiff + 1, random);
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

void Game::DefaultMemberGetInput(Position* outPos, const wchar_t* prompt) // game thread only!
{
    synchronizedTextString = prompt;
    WaitForSingleObject(inputEvent, INFINITE);
    outPos->xTile = synchronizedPos.xTile;
    outPos->yTile = synchronizedPos.yTile;
    synchronizedTextString = nullptr;
    ResetEvent(inputEvent);
}

const wchar_t* Game::GetOutputText()
{
    return synchronizedTextString;
}

void Game::showText(const wchar_t* textString)
{
    synchronizedTextString = textString;
    ResetEvent(textEvent);
    WaitForSingleObject(textEvent, INFINITE);
    synchronizedTextString = nullptr;
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

void DefaultInputFunc(void* pGameVoid, Position* outPos, const wchar_t* prompt) // game thread only!
{
    Game* game = reinterpret_cast<Game*>(pGameVoid);
    game->DefaultMemberGetInput(outPos, prompt);
}

DWORD WINAPI GameThreadEntryProc(void* pGameVoid)
{
    Game* game = reinterpret_cast<Game*>(pGameVoid);
    game->start();
    return 0;
}
