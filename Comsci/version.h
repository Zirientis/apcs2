#pragma once

#define RELEASE_VER L"0.3"
#define RELEASE_TYPE L"alpha"

#ifdef _DEBUG
#define COMSCI_VERSION_STR __DATE__ L" " __TIME__ RELEASE_VER L"-debug"
#else
#define COMSCI_VERSION_STR RELEASE_VER RELEASE_TYPE L" (" __DATE__ L" " __TIME__ L")"
#endif
