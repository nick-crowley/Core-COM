/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "com/Guid.h"
#include "com/Function.h"
#include "com/KnownGuidCollection.h"
#include "nstd/Functional.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
using namespace core;

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Variables o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Local Variables o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace
{
	auto constexpr 
	coCreateGuid = com::function<1>(::CoCreateGuid);

	auto constexpr 
	stringFromCLSID = com::function<1>(::StringFromCLSID);

	auto constexpr 
	clsIdFromProgId = com::function<1>(::CLSIDFromProgID);
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Construction & Destruction o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Copy & Move Semantics o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Static Methods o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
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
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Instance Methods & Operators o=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

com::wstring
com::Guid::wstr() const
{
    return wstring{adopt, stringFromCLSID(this->Value)};
}

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Non-member Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

std::string
com::to_string(Guid const& g)
{
	if (com::KnownGuids.contains(g.Value)) 
		return std::string{com::KnownGuids[g]};
	
	return as_string(static_cast<std::wstring_view>(g.wstr()));
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

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

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

bool
operator<(::GUID const& l, ::GUID const& r) {
	return l.Data1 < r.Data1
		&& l.Data2 < r.Data2
		&& l.Data3 < r.Data3
		&& std::array{l.Data4} < std::array{r.Data4};
}

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o