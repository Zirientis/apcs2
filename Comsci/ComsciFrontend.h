#pragma once
#include "ComsciBackend.h"

public class ComsciFrontend
{
public:
	ComsciFrontend();
	ComsciFrontend(ComsciBackend);
	~ComsciFrontend();
	
	enum BackendReusePolicy
	{
		REUSE_ALLOWED;
		PRIVATE;
	}
	
	setBackendReusePolicy(BackendReusePolicy);	
private:
	ComsciBackend* m_pBackend;
	BackendReusePolicy m_backendReusePolicy;
}
