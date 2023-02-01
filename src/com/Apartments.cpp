#include "com/Apartments.h"
#include "com/Function.h"
using namespace core;

auto constexpr 
intern coInitializeEx = com::function(CoInitializeEx);

com::IsolatedApartment::IsolatedApartment() 
{
	coInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

com::SharedApartment::SharedApartment() 
{
	coInitializeEx(nullptr, COINIT_MULTITHREADED );
}
