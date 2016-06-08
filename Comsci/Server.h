#pragma once
#include <asio.hpp>
#include "autogen/network/comsci.pb.h"

DWORD WINAPI ServerThreadEntryProc(void*)
{
    asio::io_service iosvc;
    return 0;
}
