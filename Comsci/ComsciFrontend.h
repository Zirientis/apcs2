#pragma once
#include "ComsciBackend.h"

class ComsciFrontend
{
public:
	ComsciFrontend();
	ComsciFrontend(ComsciBackend*);
	~ComsciFrontend();
	
	enum BackendReusePolicy
	{
		BACKEND_REUSE_ALLOWED,
		BACKEND_PRIVATE
	};
	
	void setBackendReusePolicy(BackendReusePolicy);
private:
	ComsciBackend* m_pBackend;
	BackendReusePolicy m_backendReusePolicy;
};
