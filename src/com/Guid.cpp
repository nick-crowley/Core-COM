#include "com/Guid.h"
#include "com/Function.h"
#include "com/KnownGuidCollection.h"
#include "nstd/Functional.h"
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
    return wstring{adopt, stringFromCLSID(this->Value)};
}

std::string
com::to_string(Guid const& g)
{
	if (com::KnownGuids.contains(g.Value)) 
		return std::string{com::KnownGuids[g]};
	
	return core::to_string(static_cast<std::wstring_view>(g.wstr()));
}

std::wstring
com::to_wstring(Guid const& g)
{
	if (com::KnownGuids.contains(g.Value)) {
		auto const s = com::KnownGuids[g.Value];
		return {s.begin(), s.end()};
	}

	return std::wstring{static_cast<std::wstring_view>(g.wstr())};
}

std::string 
to_string(::GUID const& g)
{
	if (com::KnownGuids.contains(g)) 
		return std::string{com::KnownGuids[g]};

	auto const ws = com::Guid{g}.wstr();
	return {
		boost::make_transform_iterator(ws.begin(), nstd::convert_to<char>),
		boost::make_transform_iterator(ws.end(), nstd::convert_to<char>)
	};
}

std::wstring 
to_wstring(::GUID const& g)
{
	if (com::KnownGuids.contains(g)) {
		auto const s = com::KnownGuids[g];
		return {s.begin(), s.end()};
	}

	return std::wstring{static_cast<std::wstring_view>(core::com::Guid{g}.wstr())};
}
