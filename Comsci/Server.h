#pragma once
#include <asio.hpp>
#include "autogen/network/comsci.pb.h"

#include "GameObject.h"

#ifdef _DEBUG
#pragma comment(lib, "libprotobuf.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#endif
#define COMSCI_SERVER_PORT 16180 // 10000 * phi

using namespace asio::ip;

DWORD WINAPI ServerThreadEntryProc(void* lpGameVoid)
{
    Game* game = reinterpret_cast<Game*>(lpGameVoid);
    try
    {
        asio::io_service iosvc;
        tcp::acceptor acceptor(iosvc, tcp::endpoint(tcp::v4(), COMSCI_SERVER_PORT));
        for (;;) // ever
        {
            tcp::socket sock(iosvc);
            acceptor.accept(sock);
            std::vector<char> data;
            asio::error_code err;
            asio::read(sock, asio::buffer(data), err);
        }
    }
    catch (std::exception e)
    {
        MessageBox(NULL, L"The server encountered an unexpected error.", L"Comsci", 0);
    }
    return 2;
}
