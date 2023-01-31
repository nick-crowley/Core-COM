#include "com/Guid.h"
#include "com/Function.h"
using namespace core;

namespace
{
	auto constexpr 
	coCreateGuid = com::function<1>(::CoCreateGuid);

	auto constexpr 
	stringFromCLSID = com::function<1>(::StringFromCLSID);

	auto constexpr 
	clsIdFromString = com::function<1>(::CLSIDFromString);

	auto constexpr 
	clsIdFromProgId = com::function<1>(::CLSIDFromProgID);
}

constexpr 
com::Guid::Guid(::GUID const& g) noexcept 
  : m_value{g}
{
}

//Guid::reference constexpr
//Guid::operator=(::GUID const& g) noexcept
//{
//  FIXME: Consuming modules receive 'LNK2001 unresolved external symbol' unless defined in header
//	this->m_value = g;
//	return *this;
//}

//constexpr
//Guid::operator ::GUID const&() const noexcept
//{
//  FIXME: Consuming modules receive 'LNK2001 unresolved external symbol' unless defined in header
//	return this->m_value;
//}

		
com::Guid 
com::Guid::fromProgId(std::wstring_view str)
{
	return Guid{ clsIdFromProgId(str.data()) };
}

com::Guid 
com::Guid::fromString(std::wstring_view str)
{
	return Guid{ clsIdFromString(str.data()) };
}

com::Guid
com::Guid::generate()
{
	return Guid{ coCreateGuid() };
}

com::wstring
com::Guid::str() const
{
    return wstring{adopt, stringFromCLSID(this->m_value)};
}

bool 
com::Guid::operator==(type const& r) const noexcept 
{
	return ::IsEqualGUID(this->m_value, r.m_value);
}

bool 
com::Guid::operator!=(type const& r) const noexcept 
{
	return !(*this == r);
}

std::wstring
com::to_wstring(Guid const& g)
{
	auto s = g.str();
	return {s.begin(), s.end()};
}

com::Guid
com::literals::operator""_guid(wchar_t const* guid, size_t count)
{
	return Guid::fromString({guid, guid+count});
}

bool 
operator==(::GUID const& l, com::Guid const& r) noexcept
{
	return com::Guid(l) == r;
}

bool 
operator!=(::GUID const& l, com::Guid const& r) noexcept
{
	return com::Guid(l) != r;
}

bool 
operator==(com::Guid const& l, ::GUID const& r) noexcept
{
	return l == com::Guid(r);
}

bool 
operator!=(com::Guid const& l, ::GUID const& r) noexcept
{
	return l != com::Guid(r);
}
