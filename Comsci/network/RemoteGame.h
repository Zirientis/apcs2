#pragma once
#include "../Game.h"
#include <asio.hpp>
#include "../autogen/network/comsci.pb.h"

class RemoteGame : private Game
{
private:
    void(*inputFunc) (void*, Position*, const wchar_t*);
    bool ready;

    RemoteGame(void(*getInputFunc) (void*, Position*, const wchar_t*))
        : Game()
    {
        ready = false;
        inputFunc = getInputFunc;
    }

    void start()
    {
        //
    }

    ~RemoteGame()
    {
        ready = false;
    }
};
