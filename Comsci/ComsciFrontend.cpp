#include "ComsciFrontend.h"

#include "ComsciBackend.h"
#include <new>
#include <iostream>

ComsciFrontend::ComsciFrontend()
{
	std::cout << "Creating ComsciFrontend" << std::endl;
	m_pBackend = new ComsciBackend();
	m_backendReusePolicy = BACKEND_PRIVATE;
}

ComsciFrontend::ComsciFrontend(ComsciBackend* b)
{
	std::cout << "Creating ComsciFrontend" << std::endl;
	m_pBackend = b;
	m_backendReusePolicy = ComsciFrontend::BACKEND_REUSE_ALLOWED;
}

ComsciFrontend::~ComsciFrontend()
{
	std::cout << "Destroying ComsciFrontend" << std::endl;
	if (m_backendReusePolicy == ComsciFrontend::BACKEND_PRIVATE)
	{
		delete m_pBackend;
		m_pBackend = 0;
	}
}

void ComsciFrontend::setBackendReusePolicy(BackendReusePolicy pol)
{
	m_backendReusePolicy = pol;
}

int main()
{
	{
		ComsciFrontend c = ComsciFrontend();
	}
	{
		ComsciFrontend c = ComsciFrontend();
		c.setBackendReusePolicy(ComsciFrontend::BACKEND_REUSE_ALLOWED);
	} // memory leak!
	{
		ComsciBackend b = ComsciBackend();
		ComsciFrontend c = ComsciFrontend(&b);
	} // potentially unsafe! use-after-free if b is destroyed and c returned/stashed!
	{
		ComsciFrontend c = ComsciFrontend(new ComsciBackend());
	} // pointless but safe; equivalent to the first block
	return 0;
}
