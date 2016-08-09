#include "Game.h"

#include "GameObject.h"
#include "ObjectCode.h"
#include "ActionCode.h"
#include "Position.h"
#include "LevelType.h"
#include "GameType.h"

#include "version.h"



#include <vector>
#include <random>
#include <cmath>
#include <string>

inline void AssertPositionChangeValid(Position start, Position end)
{
    const int xDiff = ((int)start.xTile) - ((int)end.xTile);
    if (std::abs(xDiff) > 1)
        __debugbreak();
    const int yDiff = ((int)start.yTile) - ((int)end.yTile);
    if (std::abs(yDiff) > 1)
        __debugbreak();
}

Game::Game(int numPlayers, GameType mode, void(*getInputFunc) (void*, Position*, const wchar_t*))
{
    ready = false;
    std::random_device rd;
    random = std::mt19937(rd());
    score = 0;
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
    gameType = mode;
}

Game::~Game()
{
    
    ready = false;
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
    showBanner();
    for (uint64_t turn = 0;;turn++) // forever
    {
        tick(turn);
        score--;
    } // forever
}

void Game::tick(uint64_t turn)
{
    const unsigned int width = m_pCurrentLevel->GetWidth();
    const unsigned int height = m_pCurrentLevel->GetHeight();
    for (m_activePlayer = 0; m_activePlayer < m_numPlayers; m_activePlayer++)
    {
        // Set the necessary overlays
        Position playerPos = m_pPlayerPositions[m_activePlayer];
        unsigned int ptrOffset = playerPos.yTile * width + playerPos.xTile;
        GameObject* overlay = m_pCurrentLevel->GetOverlays() + ptrOffset;
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
            (playersPlaced && (xDiff > 1 || yDiff > 1)) && !WIZARD_MODE);
        // deactivate the overlay

        ObjectCode targetEntCode = m_pCurrentLevel->GetEntities()[p.yTile * width + p.xTile].getCode();
        ObjectCode targetSurfCode = m_pCurrentLevel->GetSurfaces()[p.yTile * width + p.xTile].getCode();
        ActionCode actRes = AC_MAX;
        if (!playersPlaced)
        {
            GameObject playerTemplate = GameObject((ObjectCode)(ObjectCode::PLAYER_1 + m_activePlayer));
            if (placeEntity(playerTemplate, p, false) == AC_NONE)
            {
                m_pPlayerPositions[m_activePlayer] = p;
                actRes = AC_NONE;
            }
            else
            {
                showText(L"Try again!");
                m_activePlayer--;
                continue;
            }
        }
        else
            actRes = moveEntity(playerPos, p);
        if (actRes == AC_MAX)
            __debugbreak();
        if (actRes == AC_STAIR_TRIGGERED)
        {
            return;
        }
        else if (actRes == AC_NONE)
        {
            m_pPlayerPositions[m_activePlayer] = p;
        }
        else if (IsCodeCoin(targetEntCode))
        {
            score += GetScoreChange(targetEntCode);
            m_pCurrentLevel->GetEntities()[p.yTile * width + p.xTile] = GameObject();
            if (moveEntity(playerPos, p) != AC_NONE)
            {
                // ASSERT: Couldn't move to square after taking potion!
                __debugbreak();
            }
            else
                m_pPlayerPositions[m_activePlayer] = p;
            showText(L"You pick up the coins.");
        }
        else if (IsCodePotion(targetEntCode))
        {
            score += GetScoreChange(targetEntCode);
            m_pCurrentLevel->GetEntities()[p.yTile * width + p.xTile] = GameObject();
            if (moveEntity(playerPos, p) != AC_NONE)
            {
                // ASSERT: Couldn't move to square after taking potion!
                __debugbreak();
            }
            else
                m_pPlayerPositions[m_activePlayer] = p;
            showText(L"You quaff the potion. You feel a vague sense of improvement.");
        }
        else if (p == playerPos)
        {
            showText(L"You look around curiously.");
        }
        else if (IsCodePlayer(targetEntCode))
        {
            showText(L"You can't attack your teammate!");
        }
        else if (p != playerPos && !IsCodeWall(targetSurfCode))
        {
            // attack
            GameObject* npc = m_pCurrentLevel->GetEntities() + (p.yTile * width + p.xTile);
            if (npc->isAttackable())
            {
                showText(L"You attack the creature!");
                if (npc->attack(PLAYER_DAMAGE) < 0)
                {
                    showText(L"You have mortally wounded it!");
                    score += GetScoreChange(targetEntCode);
                    int randRes = random() & 0b1111;
                    if (randRes <= 0b0111)
                        *npc = GameObject(ObjectCode::COIN);
                    else if (randRes <= 0b1001 && gameType != GT_SNEK)
                        *npc = GameObject(GetSpawner(npc->getCode()));
                    else if (randRes == 0b1111)
                        *npc = GameObject(ObjectCode::POTION_PURPLE);
                    else
                        *npc = GameObject();
                }
            }
            else
                showText(L"You can't attack that!");
        }
        else
            showText(L"Something was already there! You forfeit your turn.");
    }
    playersPlaced = true;
    // AI logic runs here
    //MessageBox(NULL, LPCWSTR(u"AI Thinking..."), LPCWSTR(u"AI Thinking"), 0);
    showText(L"You hear the creatures of the dungeon begin to stir...");

    // Now allow each non-player entity to act
    // FIXME: GameObject should take a turnCreated, if objects should sleep for 1 turn.
    for (unsigned int i = 0; i < width * height; i++)
    {
        GameObject* npc = m_pCurrentLevel->GetEntities() + i;
        if (npc->isActionPerformed())
            continue;
        ObjectCode npcCode = npc->getCode();
        Position npcPos = Position{ i % width, i / width };
        if (IsCodeNone(npcCode) || IsCodePlayer(npcCode))
            continue;
        // Now do something!
        // On death, monster *should* have a chance to turn into a spawner.
        if (npc->getHealth() <= 0)
        {
            continue;
        }
        else if (IsCodeMonst(npcCode))
        {
            if (npcPos.xTile > 0 && npcPos.xTile < width - 1 && npcPos.yTile > 0 && npcPos.yTile < height - 1)
            {
                Position movePos = Position{ npcPos.xTile + (random() % 3) - 1, npcPos.yTile + (random() % 3) - 1 };
                AssertPositionChangeValid(npcPos, movePos);
                if (moveEntity(npcPos, movePos) != AC_NONE)
                {
                    GameObject* blockingEntity = m_pCurrentLevel->GetEntities() + (movePos.yTile * width + movePos.xTile);
                    if (IsCodePlayer(blockingEntity->getCode()))
                    {
                        showText(L"The creature attacks you!");
                        int lostHealth = (random() % 4) + 1;
                        int hp = blockingEntity->attack(lostHealth);
                        std::wstring dmgStr = L"You lost ";
                        dmgStr += std::to_wstring(lostHealth);
                        dmgStr += L" health. You have ";
                        dmgStr += std::to_wstring(hp);
                        dmgStr += L" health remaining.";
                        showText(dmgStr.data());
                        REAPER(hp); // We may need to end the game.
                    }
                }
            }
        }
    }
    unsigned int monstCount = 0;
    for (unsigned int i = 0; i < width * height; i++)
    {
        GameObject* npc = m_pCurrentLevel->GetEntities() + i;
        ObjectCode npcCode = npc->getCode();
        Position npcPos = Position{ i % width, i / width };
        if (IsCodeNone(npcCode) || IsCodePlayer(npcCode))
            continue;
        else if (IsCodeMonst(npcCode))
            monstCount++;
        else if (IsCodeSpawner(npcCode))
        {
            if (random() & 1) // 50% chance a spawner will activate
                continue;
            if (npcPos.xTile > 0 && npcPos.xTile < width - 1 && npcPos.yTile > 0 && npcPos.yTile < height - 1 && turn % 4 == 0)
            {
                //npc->setCode(GetSpawnedItem(npcCode));
                Position spawneePos = Position{ npcPos.xTile + (random() % 3) - 1, npcPos.yTile + (random() % 3) - 1 };
                AssertPositionChangeValid(npcPos, spawneePos);
                ActionCode res = placeEntity(GameObject(GetSpawnedItem(npcCode)), spawneePos, false);
                if (res != AC_PLACE_FAIL)
                    npc->setActionPerformed(true);
                if (DEBUG_SPAWNER && res != AC_PLACE_FAIL)
                {
                    m_pCurrentLevel->GetOverlays()[npcPos.yTile * width + npcPos.xTile].setCode(INDICATOR_RED);
                    m_pCurrentLevel->GetOverlays()[spawneePos.yTile * width + spawneePos.xTile].setCode(INDICATOR_GREEN);
                    std::wstring str = L"Spawner at (";
                    str += std::to_wstring(npcPos.xTile);
                    str += L", ";
                    str += std::to_wstring(npcPos.yTile);
                    str += L") created object at (";
                    str += std::to_wstring(spawneePos.xTile);
                    str += L", ";
                    str += std::to_wstring(spawneePos.yTile);
                    str += L").";
                    MessageBox(NULL, str.data(), L"Comsci DEBUG", 0);
                    m_pCurrentLevel->GetOverlays()[npcPos.yTile * width + npcPos.xTile].setCode(NONE);
                    m_pCurrentLevel->GetOverlays()[spawneePos.yTile * width + spawneePos.xTile].setCode(NONE);
                }
            }
        }
    }
    if (monstCount == 0 && gameType == GT_SNEK)
    {
        std::wstring msg = L"Congratulations! You killed all the Sneks in ";
        msg += std::to_wstring(turn);
        msg += L" turns!";
        MessageBox(NULL, msg.data(), L"Comsci", 0);
        PostQuitMessage(0);
    }
    m_pCurrentLevel->MarkAllEntitiesReady();
}

bool Game::IsReady()
{
    return ready;
}

ActionCode Game::moveEntity(Position start, Position end)
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
        return AC_NONE; // Nothing happens!
    }

    if (IsCodeNone(m_pCurrentLevel->GetEntityAt(end)->getCode())) // tile is empty
    {
        GameObject* oldEnt = m_pCurrentLevel->GetEntities() + (start.yTile * m_pCurrentLevel->GetWidth() + start.xTile);
        GameObject* newSurf = m_pCurrentLevel->GetSurfaces() + (end.yTile * m_pCurrentLevel->GetWidth() + end.xTile);
        GameObject* oldFurn = m_pCurrentLevel->GetFurnishings() + (start.yTile * m_pCurrentLevel->GetWidth() + start.xTile);

        if (newSurf->onBeforeWalk(oldEnt) && oldFurn->onAfterWalk(oldEnt))
        // ok to move to new tile, and old tile allows it (after effects from furnishing run)
        {
            GameObject* newEnt = m_pCurrentLevel->GetEntities() + (end.yTile * m_pCurrentLevel->GetWidth() + end.xTile);
            *newEnt = *oldEnt;
            newEnt->setActionPerformed(true);
            *oldEnt = GameObject();
            GameObject* newFurn = m_pCurrentLevel->GetFurnishings() + (end.yTile * m_pCurrentLevel->GetWidth() + end.xTile);
            ActionCode result = newFurn->onWalk(newEnt); // TODO: has issues if the trap moves the entity
            if (DEBUG_MOVE)
            {
                m_pCurrentLevel->GetOverlays()[start.yTile * m_pCurrentLevel->GetWidth() + start.xTile].setCode(INDICATOR_RED);
                m_pCurrentLevel->GetOverlays()[end.yTile * m_pCurrentLevel->GetWidth() + end.xTile].setCode(INDICATOR_GREEN);
                std::wstring debugStr = L"Move from (";
                debugStr += std::to_wstring(start.xTile);
                debugStr += L", ";
                debugStr += std::to_wstring(start.yTile);
                debugStr += L") to (";
                debugStr += std::to_wstring(end.xTile);
                debugStr += L", ";
                debugStr += std::to_wstring(end.yTile);
                debugStr += L")";
                MessageBox(NULL, debugStr.data(), L"Comsci DEBUG", 0);
                m_pCurrentLevel->GetOverlays()[start.yTile * m_pCurrentLevel->GetWidth() + start.xTile].setCode(NONE);
                m_pCurrentLevel->GetOverlays()[end.yTile * m_pCurrentLevel->GetWidth() + end.xTile].setCode(NONE);
            }
            if (result == AC_STAIR_TRIGGERED)
            {
                ObjectCode npcCode = newEnt->getCode();
                if (doStairAction(npcCode))
                    return AC_STAIR_TRIGGERED;
            }
            return AC_NONE;
        }
    }
    return AC_MOVE_FAIL;
}

ActionCode Game::placeEntity(GameObject& templateObj, Position pos, bool force)
{
    GameObject* ent = m_pCurrentLevel->GetEntities() + (pos.yTile * m_pCurrentLevel->GetWidth() + pos.xTile);
    GameObject* surf = m_pCurrentLevel->GetSurfaces() + (pos.yTile * m_pCurrentLevel->GetWidth() + pos.xTile);
    if (force || (IsCodeNone(ent->getCode()) && surf->onBeforeWalk(nullptr)))
    {
        *ent = GameObject(templateObj);
        GameObject* furn = m_pCurrentLevel->GetFurnishings() + (pos.yTile * m_pCurrentLevel->GetWidth() + pos.xTile);
        furn->onWalk(ent);
        return AC_NONE;
    }
    return AC_PLACE_FAIL;
}

void Game::showBanner()
{
    showText(L"Welcome to Comsci\u2122! [Build " COMSCI_VERSION_STR "]");
    showText(L"Please read the ReadMe if you have not already done so.");
    std::wstring gameStr = L"Game mode is ";
    gameStr += GetGameTypeStringW(gameType);
    showText(gameStr.data());
    showText(L"Objectives:");
    switch (gameType)
    {
    case GT_SPIDER:
        showText(L"(1/2) Survive the spider onslaught!");
        showText(L"(2/2) Get a high score!");
        break;
    case GT_SNEK:
        showText(L"Destroy all the SNEKs!");
        break;
    case GT_CLASSIC:
        showText(L"Begin your adventure!");
    default:
        break;
    }
}

void Game::advanceLevel()
{
    ready = false;
    int oldDiff = -1;
    if (m_pCurrentLevel)
    {
        oldDiff = m_pCurrentLevel->GetDifficulty();
        delete m_pCurrentLevel;
    }
    m_pCurrentLevel = new Level(oldDiff + 1, LevelTypeFromGameType(gameType), random());
    ready = true;
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
        //__debugbreak();
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

const int Game::GetDifficulty()
{
    return m_pCurrentLevel->GetDifficulty();
}

const int64_t Game::GetScore()
{
    return score;
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

inline void Game::DEATH()
{
    // HACKETY HACK HACK
    std::wstring outstr;
    outstr += L"Game Over! Your score was ";
    outstr += std::to_wstring(score);
    MessageBox(NULL, outstr.data(), L"Comsci", 0);
    //delete this;
    ExitThread(0);
}

inline void Game::REAPER(int hp)
{
    if (hp <= 0)
    {
        showText(L"You have died!");
        DEATH();
    }
}

// If this returns true, the level has been invalidated.
inline bool Game::doStairAction(ObjectCode triggerCode)
{
    switch (gameType)
    {
    case GT_SPIDER:
        if (IsCodePlayer(triggerCode))
        {
            //return AC_STAIR_TRIGGERED;
            showText(L"The stairs collapse downward!");
            showText(L"It seems that your adventure is over!");
            DEATH();
        }
        break;
    case GT_CLASSIC:
        // here we go
        if (IsCodePlayer(triggerCode))
        {
            showText(L"You descend the stairs.");
            score += GetScoreChange(STAIRS);
            advanceLevel();
            Position playerPos = LevelGenerator::randPlaceOneInLevel(m_pCurrentLevel, m_pCurrentLevel->GetEntities(), &random, GameObject(PLAYER_1));
            m_pPlayerPositions[m_activePlayer] = playerPos;
            return true;
        }
        break;
    default:
        break;
    }
    return false;
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
