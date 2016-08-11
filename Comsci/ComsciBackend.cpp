#include "ComsciBackend.h"
#include <iostream>

ComsciBackend::ComsciBackend()
{
	std::cout << "Creating ComsciBackend" << std::endl;
}

ComsciBackend::~ComsciBackend()
{
	std::cout << "Destroying ComsciBackend" << std::endl;
}

void ComsciBackend::close()
{
	std::cout << "Closing ComsciBackend" << std::endl;
}
