#pragma once
#include "../Game.h"
#include "../Level.h"
#include <asio.hpp>
#include "../Server.h"

using namespace asio::ip;
#pragma comment(lib, "libprotobuf.lib")

class RemoteGame : public Game
{
private:
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
    void(*inputFunc) (void*, Position*, const wchar_t*);
    bool ready;


public:
    RemoteGame(void(*getInputFunc) (void*, Position*, const wchar_t*))
        : Game()
    {
        MessageBox(NULL, L"You see a wooden sign. Someone with impeccable penmanship\n has painted in white paint: NOT YET IMPLEMENTED.", L"Comsci", MB_ICONINFORMATION);
        /*
        ready = false;
        asio::io_service svc;
        tcp::socket sock(svc);
        tcp::resolver res(svc);
        tcp::resolver::query q("127.0.0.1", std::to_string(COMSCI_SERVER_PORT));
        asio::connect(sock, res.resolve(q));
        csnet::ClientHello hello;
        hello.set_version(0);
        std::string out = hello.SerializeAsString();
        //hello.SerializeToString(&out);
        asio::error_code err;
        size_t msgLen = out.length();
        asio::write(sock, asio::buffer(&msgLen, sizeof(size_t)), err);
        size_t len = asio::write(sock, asio::buffer(out), err);
        inputFunc = getInputFunc;
        //ready = true;
        */
    }

    ~RemoteGame()
    {
        ready = false;
        delete m_pCurrentLevel;
        m_pCurrentLevel = nullptr;

        delete[] m_pPlayerPositions;
        m_pPlayerPositions = nullptr;
        inputFunc = nullptr;
        synchronizedPos.xTile = 0;
        synchronizedPos.yTile = 0;
        CloseHandle(inputEvent);
        inputEvent = nullptr;
        CloseHandle(textEvent);
        textEvent = nullptr;
    }

    bool IsReady()
    {
        return ready;
    }

    void start()
    {
        // connect to the remote Server thread


        //ready = true;
    }

    const wchar_t* RemoteGame::GetOutputText()
    {
        return synchronizedTextString;
    }

    int RemoteGame::getActivePlayer() // main thread, and only when locked
    {
        return m_activePlayer;
    }

    const int RemoteGame::GetDifficulty()
    {
        return m_pCurrentLevel->GetDifficulty();
    }

    const int64_t RemoteGame::GetScore()
    {
        return score;
    }

    const GameObject* RemoteGame::GetEntityAt(Position p)
    {
        return m_pCurrentLevel->GetEntityAt(p);
    }

    const GameObject* RemoteGame::GetFurnishingAt(Position p)
    {
        return m_pCurrentLevel->GetFurnishingAt(p);
    }

    const GameObject* RemoteGame::GetSurfaceAt(Position p)
    {
        return m_pCurrentLevel->GetSurfaceAt(p);
    }

    const GameObject* RemoteGame::GetOverlayAt(Position p)
    {
        return m_pCurrentLevel->GetOverlayAt(p);
    }

    unsigned int RemoteGame::GetWidth()
    {
        return m_pCurrentLevel->GetWidth();
    }

    unsigned int RemoteGame::GetHeight()
    {
        return m_pCurrentLevel->GetHeight();
    }

    bool RemoteGame::MaybeSendPosition(Position pos) // main thread only!
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

    void RemoteGame::DefaultMemberGetInput(Position* outPos, const wchar_t* prompt) // game thread only!
    {
        synchronizedTextString = prompt;
        WaitForSingleObject(inputEvent, INFINITE);
        outPos->xTile = synchronizedPos.xTile;
        outPos->yTile = synchronizedPos.yTile;
        synchronizedTextString = nullptr;
        ResetEvent(inputEvent);
    }
};
