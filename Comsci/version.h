#pragma once
#ifdef _DEBUG
#define COMSCI_VERSION_STR __DATE__ L" " __TIME__
#else
#define COMSCI_VERSION_STR L"0.3alpha (" __DATE__ L" " __TIME__ L")"
#endif
