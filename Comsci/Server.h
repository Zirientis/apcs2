#pragma once
#include <boost/asio.hpp>

#include "GameObject.h"

#define COMSCI_SERVER_PORT 16180 // 10000 * phi

using namespace boost::asio;
using namespace boost::asio::ip;


DWORD WINAPI CoreThreadEntryProc()
{
    try
    {
        io_service iosvc;
        tcp::acceptor acceptor4(iosvc, tcp::endpoint(tcp::v4(), COMSCI_SERVER_PORT));
        for (;;) // ever
        {
        }
    }
    catch (std::exception e)
    {
        MessageBox(NULL, L"The server encountered an unexpected error.", L"Comsci", 0);
    }
    return 2;
}
