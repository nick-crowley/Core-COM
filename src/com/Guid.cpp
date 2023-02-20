#include "com/Guid.h"
#include "com/Function.h"
#include "com/KnownGuidCollection.h"
#include "core/ToString.h"
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
com::Guid::wstr() const
{
	if (com::KnownGuids.contains(this->m_value)) {
		auto ws = com::KnownGuids[this->m_value];
		return {ws.begin(), ws.end()};
	}

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

std::string
com::to_string(Guid const& g)
{
	return core::to_string(static_cast<std::wstring_view>(g.wstr()));
}

std::wstring
com::to_wstring(Guid const& g)
{
	return std::wstring{static_cast<std::wstring_view>(g.wstr())};
}

com::Guid
com::literals::operator""_guid(gsl::cwzstring guid, size_t count)
{
	return Guid::fromString({guid, guid+count});
}

std::string 
to_string(::GUID const& g)
{
	auto ws = com::Guid{g}.wstr();
	return {ws.begin(), ws.end()};
}

std::wstring 
to_wstring(::GUID const& g)
{
	return std::wstring{static_cast<std::wstring_view>(core::com::Guid{g}.wstr())};
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

bool
operator<(::GUID const& l, ::GUID const& r) {
	return l.Data1 < r.Data1
		&& l.Data2 < r.Data2
		&& l.Data3 < r.Data3
		&& l.Data4 < r.Data4;
}
