#include "comFramework.h"
#include "com/Exception.h"
using namespace core;

void 
com::throw_exception(::HRESULT hr, std::source_location loc)
{
	throw std::system_error{hr, std::system_category()};
}

void 
com::throw_exception(::HRESULT hr, std::string_view msg, std::source_location loc)
{
	throw std::system_error{hr, std::system_category(), msg.data()};
}
