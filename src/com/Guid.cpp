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
	clsIdFromProgId = com::function<1>(::CLSIDFromProgID);
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

std::string 
to_string(::GUID const& g)
{
	auto const ws = com::Guid{g}.wstr();
	return {
		boost::make_transform_iterator(ws.begin(), nstd::convert_to<char>),
		boost::make_transform_iterator(ws.end(), nstd::convert_to<char>)
	};
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
