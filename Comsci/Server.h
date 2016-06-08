#pragma once
#include <asio.hpp>
#include "autogen/network/comsci.pb.h"

#include "GameObject.h"

DWORD WINAPI ServerThreadEntryProc(void* lpGameVoid)
{
    Game* game = reinterpret_cast<Game*>(lpGameVoid);
    
    asio::io_service iosvc;
    return 2;
}
