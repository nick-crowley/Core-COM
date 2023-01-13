#include "comFramework.h"
#include "com/Exception.h"
using namespace core;

std::string 
extern formatMessage(int err);

void 
com::throw_exception(::HRESULT hr, std::source_location loc)
{
	throw std::runtime_error{formatMessage(hr)};
}
