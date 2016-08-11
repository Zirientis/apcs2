#include "ComsciFrontend.h"

#include "ComsciBackend.h"

ComsciFrontend::ComsciFrontend()
{
	m_pBackend = new ComsciBackend();
	m_backendReusePolicy = BackendReusePolicy::PRIVATE;
}

ComsciFrontend::ComsciFrontend(ComsciBackend* b)
{
	m_pBackend = b;
	m_backendReusePolicy = BackendReusePolicy::REUSE_ALLOWED;
}