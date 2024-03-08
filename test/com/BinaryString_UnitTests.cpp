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
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
//#define DISABLE_BINARY_STRING_UNIT_TESTS
#ifndef DISABLE_BINARY_STRING_UNIT_TESTS
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include <gtest/gtest.h>
#include "com/BinaryString.h"
#include <string.h>
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
using namespace core;
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
TEST(BinaryString_UT, DefaultConstructor_ConstructsNullBSTR) 
{
	wstring obj;

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is @e null @c BSTR rather than @e empty @c BSTR
	EXPECT_EQ(nullptr, obj.c_str());
	EXPECT_EQ(nullptr, obj.data());
}

TEST(BinaryString_UT, CopyConstructor_ClonesNonEmptyString) 
{
	wstring const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", wstring{src});
}

TEST(BinaryString_UT, CopyConstructor_ClonesEmptyBStr) 
{
	wstring const src{L""};
	
	//! @test  Verify result is @e empty @c BSTR
	EXPECT_EQ(L"", wstring{src});
}

TEST(BinaryString_UT, CopyConstructor_ClonesNullBStr) 
{
	wstring const src;
	
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, wstring{src}.data());
}

TEST(BinaryString_UT, MoveConstructor_TransfersContentLeavingNullBStr) 
{
	wstring src{L"abc"};
	
	//! @test  Verify characters were moved
	EXPECT_EQ(L"abc", wstring{std::move(src)});

	//! @post  Input string is now @e null @c BSTR
	EXPECT_TRUE(src.empty());
	EXPECT_EQ(nullptr, src.data());
}

TEST(BinaryString_UT, IteratorPairConstructor_ClonesInputRange) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify characters of source range were copied
	EXPECT_EQ(L"abc", wstring(src.begin(), src.end()));
}

TEST(BinaryString_UT, IteratorPairConstructor_ConvertsInputRange) 
{
	std::string const src{"abc"};
	
	//! @test  Verify characters of source range were converted and copied
	EXPECT_EQ(L"abc", wstring(src.begin(), src.end()));
}

TEST(BinaryString_UT, IteratorPairConstructor_RejectsInvalidRange) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify invalid argument is rejected
	EXPECT_ANY_THROW(
		wstring(src.end(), src.begin())
	);
}

TEST(BinaryString_UT, IteratorPairConstructor_RejectsMissingArgument) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify either missing argument is detected
	EXPECT_ANY_THROW(wstring(&src[0], (wchar_t const*)nullptr));
	EXPECT_ANY_THROW(wstring((wchar_t const*)nullptr, &src[1]));
}

TEST(BinaryString_UT, IteratorPairConstructor_EmptyStringWhenRangeIsEmpty) 
{
	std::wstring const src{L"abc"};
	wstring obj{src.begin(), src.begin()};

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is an @e empty @c BSTR rather than @e null @c BSTR
	EXPECT_NE(nullptr, obj.c_str());
	EXPECT_NE(nullptr, obj.data());
	EXPECT_STREQ(L"", obj.c_str());
}

TEST(BinaryString_UT, IteratorPairConstructor_NullStringWhenRangeIsBothNull) 
{
	std::wstring const src{L"abc"};
	wstring obj{(wchar_t const*)nullptr, (wchar_t const*)nullptr};

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is an @e null @c BSTR rather than @e empty @c BSTR
	EXPECT_EQ(nullptr, obj.c_str());
	EXPECT_EQ(nullptr, obj.data());
}

TEST(BinaryString_UT, AdoptConstructor_TakesOwnershipOfInputBuffer) 
{
	wchar_t* buffer = ::SysAllocString(L"abc");
	
	//! @test  Verify buffer has been transferred, not copied, into string
	EXPECT_EQ(
		(void*)buffer,
		(void*)wstring(adopt, buffer).c_str()
	);
}

TEST(BinaryString_UT, AdoptConstructor_ConstructsEmptyBStr) 
{
	wchar_t* buffer = ::SysAllocString(L"");
	
	//! @test  Verify buffer is empty
	EXPECT_EQ(0u, wcslen(buffer));
	
	//! @test  Verify buffer has been transferred, not copied, into string
	EXPECT_EQ(
		(void*)buffer,
		(void*)wstring(adopt, buffer).c_str()		
	);
}

TEST(BinaryString_UT, AdoptConstructor_ConstructsNullBStr) 
{
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(
		nullptr,
		wstring(adopt, nullptr).c_str()
	);
}

TEST(BinaryString_UT, CStringConstructor_ClonesInputBuffer) 
{
	auto const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", wstring{src});
	
	//! @test  Verify buffer has been copied, not transferred, into string
	EXPECT_NE(
		(void*)src,
		(void*)wstring(src).c_str()		
	);
}

TEST(BinaryString_UT, CStringConstructor_ConstructsEmptyBStr) 
{
	//! @test  Verify result is @e empty @c BSTR
	EXPECT_NE(nullptr, wstring(L"").c_str());
}

TEST(BinaryString_UT, CStringConstructor_ConstructsNullBStr) 
{
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, wstring((const wchar_t*)nullptr).c_str());
}

TEST(BinaryString_UT, StdStringConstructor_ClonesNonEmptyNarrowString) 
{
	std::string const src{"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", wstring{src});
}

TEST(BinaryString_UT, StdStringConstructor_ClonesNonEmptyWideString) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", wstring{src});
}

TEST(BinaryString_UT, Assign_ClonesInputRange) 
{
	std::wstring const src{L"abc"};

	//! @test  Verify characters of source range were assigned
	wstring obj{};
	obj.assign(src.begin(), src.end());
	EXPECT_TRUE(obj == L"abc");
}

TEST(BinaryString_UT, Assign_EmptyBStrWhenRangeIsEmpty) 
{
	std::wstring const src{L""};
	wstring obj{};
	obj.assign(src.begin(), src.end());

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is @e empty @c BSTR rather than @e null @c BSTR
	EXPECT_NE(nullptr, obj.data());
	EXPECT_NE(nullptr, obj.c_str());
	EXPECT_STREQ(L"", obj.c_str());
}

TEST(BinaryString_UT, Assign_NullBStrWhenRangeIsNull) 
{
	wstring obj{L"abc"};
	obj.assign((wchar_t const*)nullptr, (wchar_t const*)nullptr);

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is @e null @c BSTR rather than @e empty @c BSTR
	EXPECT_EQ(nullptr, obj.data());
	EXPECT_EQ(nullptr, obj.c_str());
}

TEST(BinaryString_UT, Assign_RejectsMissingArgument) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify either missing argument is detected
	EXPECT_ANY_THROW(wstring{}.assign(&src[0], (wchar_t const*)nullptr));
	EXPECT_ANY_THROW(wstring{}.assign((wchar_t const*)nullptr, &src[1]));
}

TEST(BinaryString_UT, Back_ReturnsLastCharacter) 
{
	//! @test  Verify result is final character
	EXPECT_EQ(L'c', wstring{L"abc"}.back());
}

TEST(BinaryString_UT, Back_ThrowsWhenEmpty) 
{
	//! @test  Verify behaviour when string is @e empty @c BSTR
	EXPECT_ANY_THROW(wstring{L""}.back());
}

TEST(BinaryString_UT, Back_ThrowsWhenNull) 
{
	//! @test  Verify behaviour when string is @e null @c BSTR
	EXPECT_ANY_THROW(wstring{}.back());
	EXPECT_ANY_THROW(wstring{(wchar_t const*)nullptr}.back());
}

TEST(BinaryString_UT, CStr_ReturnsNullTerminatedContentWhenNonEmpty) 
{
	//! @test  Verify result is accurate and null-terminated
	EXPECT_STREQ(L"abc", wstring{L"abc"}.c_str());
}

TEST(BinaryString_UT, CStr_ReturnsEmptyNullTerminatedBufferWhenEmptyBStr) 
{
	//! @test  Verify result is not @c nullptr when @e empty
	EXPECT_STREQ(L"", wstring{L""}.c_str());
	
	//! @test  Verify result is not a copy but the real buffer
	wstring src{L"abc"};
	EXPECT_EQ(src.data(), src.c_str());
}

TEST(BinaryString_UT, CStr_ReturnsNullptrWhenNullBStr)
{
	//! @test  Verify result is @c nullptr when a @e null @c BSTR
	EXPECT_EQ(nullptr, wstring{}.c_str());
	EXPECT_EQ(nullptr, wstring{(wchar_t const*)nullptr}.c_str());
}

TEST(BinaryString_UT, Data_CompatibleWithSysFreeString) 
{
	wstring src{L"abc"};

	//! @test  Verify @c SysFreeString() can free the detached BSTR
	EXPECT_NO_FATAL_FAILURE(::SysFreeString(src.detach()));
}

TEST(BinaryString_UT, Data_CompatibleWithSysStringLen) 
{
	wstring src{L"abc"};

	//! @test  Verify @c SysStringLen() can obtain the length from the BSTR
	EXPECT_EQ(3u, ::SysStringLen(src.bstr()));
}

TEST(BinaryString_UT, Data_ReturnsNullptrWhenNullBStr) 
{
	//! @test  Verify result is @c nullptr when a @e null @c BSTR
	EXPECT_EQ(nullptr, wstring{}.data());
	EXPECT_EQ(nullptr, wstring{(wchar_t const*)nullptr}.data());
}

TEST(BinaryString_UT, Data_ReturnsNonNullWhenEmptyBStr) 
{
	//! @test  Verify result is not @c nullptr when an @e empty @c BSTR
	EXPECT_NE(nullptr, wstring{L""}.data());
}

TEST(BinaryString_UT, Detach_NullWhenEmpty) 
{
	//! @test  Verify result is nullptr when empty
	EXPECT_EQ(nullptr, wstring{}.detach());
}

TEST(BinaryString_UT, Detach_ObjectIsEmptyAfterwards) 
{
	wstring src{L"abc"};
	src.detach();

	//! @test  Verify object is @e empty afterwards
	EXPECT_TRUE(src.empty());
}

TEST(BinaryString_UT, Detach_ReturnsNullTerminatedBuffer) 
{
	//! @test  Verify buffer is complete and null-terminated
	EXPECT_STREQ(L"abc", wstring{L"abc"}.detach());
}

TEST(BinaryString_UT, equality_WhenComparedToCStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(L"abc", wstring{L"abc"});
	EXPECT_EQ(wstring{L"abc"}, L"abc");

	//! @test  Verify result when unequal
	EXPECT_NE(L"abc", wstring{L"ABC"});
	EXPECT_NE(wstring{L""}, L"abc");
}

TEST(BinaryString_UT, equality_WhenComparedToComStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(wstring{L"abc"}, wstring{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(wstring{L"abc"}, wstring{L""});
	EXPECT_NE(wstring{L"ABC"}, wstring{L"abc"});
}

TEST(BinaryString_UT, equality_WhenComparedToStdStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(std::wstring{L"abc"}, wstring{L"abc"});
	EXPECT_EQ(wstring{L"abc"}, std::wstring{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(std::wstring{L"abc"}, wstring{L"ABC"});
	EXPECT_NE(wstring{L""}, std::wstring{L"abc"});
}

TEST(BinaryString_UT, equality_WhenComparedToStdStringViews) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(std::wstring_view{L"abc"}, wstring{L"abc"});
	EXPECT_EQ(wstring{L"abc"}, std::wstring_view{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(std::wstring_view{L"AB"}, wstring{L"abc"});
	EXPECT_NE(wstring{L"abc"}, std::wstring_view{L""});
}

TEST(BinaryString_UT, equality_WhenEmptyBStr) 
{	
	//! @test  Verify result is equal when both strings are @e empty @c BSTR
	EXPECT_EQ(wstring{L""}, wstring{L""});
}

TEST(BinaryString_UT, equality_WhenNullBStr) 
{	
	//! @test  Verify result is equal when both strings are @e null @c BSTR
	EXPECT_EQ(wstring{}, wstring{});
	EXPECT_EQ(wstring{(wchar_t const*)nullptr}, wstring{});
	EXPECT_EQ(wstring{}, wstring{(wchar_t const*)nullptr});
}

TEST(BinaryString_UT, equality_BetweenNullBStrAndEmptyBStr) 
{	
	//! @test  Verify result is @e equality between @e null and @e empty @c BSTR
	EXPECT_EQ(wstring{L""}, wstring{});
	EXPECT_EQ(wstring{}, wstring{L""});
	EXPECT_EQ(wstring{L""}, wstring{(wchar_t const*)nullptr});
	EXPECT_EQ(wstring{(wchar_t const*)nullptr}, wstring{L""});

	//! @remarks  This behaviour is consistent with Microsoft's @c BSTR wrapper @c _bstr_t
}

TEST(BinaryString_UT, Empty_TrueWhenNullBStr) 
{
	//! @test  Verify result is @c true when a @e null @c BSTR
	EXPECT_TRUE(wstring{}.empty());
	EXPECT_TRUE(wstring{(wchar_t const*)nullptr}.empty());
}

TEST(BinaryString_UT, Empty_TrueWhenEmptyBStr) 
{
	std::wstring const src{L"abc"};

	//! @test  Verify result is @c true when an @e empty @c BSTR
	EXPECT_TRUE(wstring{L""}.empty());
	EXPECT_TRUE(wstring(src.begin(), src.begin()).empty());
}

TEST(BinaryString_UT, Empty_FalseWhenNonEmpty) 
{
	//! @test  Verify result is @c false
	EXPECT_FALSE(wstring{L"abc"}.empty());
}

TEST(BinaryString_UT, Front_ReturnsFirstCharacter) 
{
	//! @test  Verify result is first character
	EXPECT_EQ(L'a', wstring{L"abc"}.front());
}

TEST(BinaryString_UT, Front_ThrowsWhenEmpty) 
{
	//! @test  Verify behaviour when string is @e empty @c BSTR
	EXPECT_ANY_THROW(wstring{L""}.front());
}

TEST(BinaryString_UT, Front_ThrowsWhenNull) 
{
	//! @test  Verify behaviour when string is a @e null @c BSTR
	EXPECT_ANY_THROW(wstring{}.front());
	EXPECT_ANY_THROW(wstring{(wchar_t const*)nullptr}.front());
}

TEST(BinaryString_UT, Size_ReturnsNumberOfCharsWhenNonEmpty) 
{
	wstring src{L"abc"};

	//! @test  Verify result is in characters, not bytes
	EXPECT_EQ(3u, src.size());
}

TEST(BinaryString_UT, Size_ReturnsZeroWhenEmpty) 
{
	//! @test  Verify result is zero when empty
	EXPECT_EQ(0u, wstring{L""}.size());
}

TEST(BinaryString_UT, Size_ReturnsZeroWhenNull) 
{
	//! @test  Verify result is zero when a @e null @c BSTR
	EXPECT_EQ(0u, wstring{}.size());
	EXPECT_EQ(0u, wstring{(wchar_t const*)nullptr}.size());
}

} // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#endif	// DISABLE_BINARY_STRING_UNIT_TESTS