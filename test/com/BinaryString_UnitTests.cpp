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
	bstring obj;

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is @e null @c BSTR rather than @e empty @c BSTR
	EXPECT_EQ(nullptr, obj.c_str());
	EXPECT_EQ(nullptr, obj.data());
}

TEST(BinaryString_UT, CopyConstructor_ClonesConstexprBString) 
{	
	//! @test  Verify characters are copied from string with different allocator
	EXPECT_EQ(L"abc", bstring{L"abc"_bstr});
}

TEST(BinaryString_UT, CopyConstructor_ClonesNonEmptyString) 
{
	bstring const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", bstring{src});
}

TEST(BinaryString_UT, CopyConstructor_ClonesEmptyBStr) 
{
	bstring const src{L""};
	
	//! @test  Verify result is @e empty @c BSTR
	EXPECT_EQ(L"", bstring{src});
}

TEST(BinaryString_UT, CopyConstructor_ClonesNullBStr) 
{
	bstring const src;
	
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, bstring{src}.data());
}

TEST(BinaryString_UT, CopyConstructor_ClonesWString) 
{	
	//! @test  Verify characters are copied from string with different allocator
	EXPECT_EQ(L"abc", bstring{wstring{L"abc"}});
}

TEST(BinaryString_UT, MoveConstructor_TransfersContentLeavingNullBStr) 
{
	bstring src{L"abc"};
	
	//! @test  Verify characters were moved
	EXPECT_EQ(L"abc", bstring{std::move(src)});

	//! @post  Input string is now @e null @c BSTR
	EXPECT_TRUE(src.empty());
	EXPECT_EQ(nullptr, src.data());
}

TEST(BinaryString_UT, IteratorPairConstructor_ClonesInputRange) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify characters of source range were copied
	EXPECT_EQ(L"abc", bstring(src.begin(), src.end()));
}

TEST(BinaryString_UT, IteratorPairConstructor_ConvertsInputRange) 
{
	std::string const src{"abc"};
	
	//! @test  Verify characters of source range were converted and copied
	EXPECT_EQ(L"abc", bstring(src.begin(), src.end()));
}

TEST(BinaryString_UT, IteratorPairConstructor_RejectsInvalidRange) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify invalid argument is rejected
	EXPECT_ANY_THROW(
		bstring(src.end(), src.begin())
	);
}

TEST(BinaryString_UT, IteratorPairConstructor_RejectsMissingArgument) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify either missing argument is detected
	EXPECT_ANY_THROW(bstring(&src[0], (wchar_t const*)nullptr));
	EXPECT_ANY_THROW(bstring((wchar_t const*)nullptr, &src[1]));
}

TEST(BinaryString_UT, IteratorPairConstructor_EmptyStringWhenRangeIsEmpty) 
{
	std::wstring const src{L"abc"};
	bstring obj{src.begin(), src.begin()};

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
	bstring obj{(wchar_t const*)nullptr, (wchar_t const*)nullptr};

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
		(void*)bstring(adopt, buffer).c_str()
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
		(void*)bstring(adopt, buffer).c_str()		
	);
}

TEST(BinaryString_UT, AdoptConstructor_ConstructsNullBStr) 
{
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(
		nullptr,
		bstring(adopt, nullptr).c_str()
	);
}

TEST(BinaryString_UT, CStringConstructor_ClonesInputBuffer) 
{
	auto const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", bstring{src});
	
	//! @test  Verify buffer has been copied, not transferred, into string
	EXPECT_NE(
		(void*)src,
		(void*)bstring(src).c_str()		
	);
}

TEST(BinaryString_UT, CStringConstructor_ConstructsEmptyBStr) 
{
	//! @test  Verify result is @e empty @c BSTR
	EXPECT_NE(nullptr, bstring(L"").c_str());
}

TEST(BinaryString_UT, CStringConstructor_ConstructsNullBStr) 
{
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, bstring((const wchar_t*)nullptr).c_str());
}

TEST(BinaryString_UT, StdStringConstructor_ClonesNonEmptyNarrowString) 
{
	std::string const src{"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", bstring{src});
}

TEST(BinaryString_UT, StdStringConstructor_ClonesNonEmptyWideString) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", bstring{src});
}

TEST(BinaryString_UT, CopyAssignment_ClonesConstexprBString) 
{
	bstring dest{L"abc"};
	BinaryString<NoopAllocator<wchar_t>> const lvalue{L"def"_bstr};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"def", dest = lvalue);
}

TEST(BinaryString_UT, CopyAssignment_ClonesConstexprBStringRValue) 
{
	bstring dest{L"abc"};

	//! @test  Verify characters were copied
	EXPECT_EQ(L"def", dest = L"def"_bstr);
	EXPECT_FALSE(dest.empty());
}

TEST(BinaryString_UT, CopyAssignment_ClonesNonEmptyString) 
{
	bstring dest{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"def", dest = L"def");
}

TEST(BinaryString_UT, CopyAssignment_ClonesEmptyBStr) 
{
	bstring dest{L"abc"};
	
	//! @test  Verify result is @e empty @c BSTR
	EXPECT_EQ(L"", dest = L"");
}

TEST(BinaryString_UT, CopyAssignment_ClonesNullBStr) 
{
	bstring dest{L"abc"};
	dest = bstring{};
	
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, dest.data());
}

TEST(BinaryString_UT, CopyAssignment_ClonesNullptr) 
{
	bstring dest{L"abc"};
	dest = nullptr;
	
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, dest.data());
}

TEST(BinaryString_UT, CopyAssignment_ClonesWString) 
{
	bstring dest{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"def", dest = wstring{L"def"});
}

TEST(BinaryString_UT, MoveAssignment_TransfersNonEmptyString)
{
	bstring src{L"abc"};
	bstring dest{L"def"};
	
	//! @test  Verify characters are present in destination
	EXPECT_EQ(L"abc", dest = std::move(src));

	//! @test  Verify content was moved rather than copied
	EXPECT_TRUE(src.empty());
}

TEST(BinaryString_UT, Assign_ClonesInputRange) 
{
	std::wstring const src{L"abc"};

	//! @test  Verify characters of source range were assigned
	bstring obj{};
	obj.assign(src.begin(), src.end());
	EXPECT_EQ(obj, L"abc");
}

TEST(BinaryString_UT, Assign_EmptyBStrWhenRangeIsEmpty) 
{
	std::wstring const src{L""};
	bstring obj{};
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
	bstring obj{L"abc"};
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
	EXPECT_ANY_THROW(bstring{}.assign(&src[0], (wchar_t const*)nullptr));
	EXPECT_ANY_THROW(bstring{}.assign((wchar_t const*)nullptr, &src[1]));
}

TEST(BinaryString_UT, Back_ReturnsLastCharacter) 
{
	//! @test  Verify result is final character
	EXPECT_EQ(L'c', bstring{L"abc"}.back());
}

TEST(BinaryString_UT, Back_ThrowsWhenEmpty) 
{
	//! @test  Verify behaviour when string is @e empty @c BSTR
	EXPECT_ANY_THROW(bstring{L""}.back());
}

TEST(BinaryString_UT, Back_ThrowsWhenNull) 
{
	//! @test  Verify behaviour when string is @e null @c BSTR
	EXPECT_ANY_THROW(bstring{}.back());
	EXPECT_ANY_THROW(bstring{(wchar_t const*)nullptr}.back());
}

TEST(BinaryString_UT, CStr_ReturnsNullTerminatedContentWhenNonEmpty) 
{
	//! @test  Verify result is accurate and null-terminated
	EXPECT_STREQ(L"abc", bstring{L"abc"}.c_str());
}

TEST(BinaryString_UT, CStr_ReturnsEmptyNullTerminatedBufferWhenEmptyBStr) 
{
	//! @test  Verify result is not @c nullptr when @e empty
	EXPECT_STREQ(L"", bstring{L""}.c_str());
	
	//! @test  Verify result is not a copy but the real buffer
	bstring src{L"abc"};
	EXPECT_EQ(src.data(), src.c_str());
}

TEST(BinaryString_UT, CStr_ReturnsNullptrWhenNullBStr)
{
	//! @test  Verify result is @c nullptr when a @e null @c BSTR
	EXPECT_EQ(nullptr, bstring{}.c_str());
	EXPECT_EQ(nullptr, bstring{(wchar_t const*)nullptr}.c_str());
}

TEST(BinaryString_UT, Data_CompatibleWithSysFreeString) 
{
	bstring src{L"abc"};

	//! @test  Verify @c SysFreeString() can free the detached BSTR
	EXPECT_NO_FATAL_FAILURE(::SysFreeString(src.detach()));
}

TEST(BinaryString_UT, Data_CompatibleWithSysStringLen) 
{
	bstring src{L"abc"};

	//! @test  Verify @c SysStringLen() can obtain the length from the BSTR
	EXPECT_EQ(3u, ::SysStringLen(src.bstr()));
}

TEST(BinaryString_UT, Data_ReturnsNullptrWhenNullBStr) 
{
	//! @test  Verify result is @c nullptr when a @e null @c BSTR
	EXPECT_EQ(nullptr, bstring{}.data());
	EXPECT_EQ(nullptr, bstring{(wchar_t const*)nullptr}.data());
}

TEST(BinaryString_UT, Data_ReturnsNonNullWhenEmptyBStr) 
{
	//! @test  Verify result is not @c nullptr when an @e empty @c BSTR
	EXPECT_NE(nullptr, bstring{L""}.data());
}

TEST(BinaryString_UT, Detach_NullWhenEmpty) 
{
	//! @test  Verify result is nullptr when empty
	EXPECT_EQ(nullptr, bstring{}.detach());
}

TEST(BinaryString_UT, Detach_ObjectIsEmptyAfterwards) 
{
	bstring src{L"abc"};
	src.detach();

	//! @test  Verify object is @e empty afterwards
	EXPECT_TRUE(src.empty());
}

TEST(BinaryString_UT, Detach_ReturnsNullTerminatedBuffer) 
{
	//! @test  Verify buffer is complete and null-terminated
	EXPECT_STREQ(L"abc", bstring{L"abc"}.detach());
}

TEST(BinaryString_UT, equality_WhenComparedToCStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(L"abc", bstring{L"abc"});
	EXPECT_EQ(bstring{L"abc"}, L"abc");

	//! @test  Verify result when unequal
	EXPECT_NE(L"abc", bstring{L"ABC"});
	EXPECT_NE(bstring{L""}, L"abc");
}

TEST(BinaryString_UT, equality_WhenComparedToBStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(bstring{L"abc"}, bstring{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(bstring{L"abc"}, bstring{L""});
	EXPECT_NE(bstring{L"ABC"}, bstring{L"abc"});
}

TEST(BinaryString_UT, equality_WhenComparedToConstexprBStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(bstring{L"abc"}, L"abc"_bstr);

	//! @test  Verify result when unequal
	EXPECT_NE(bstring{L"abc"}, L""_bstr);
	EXPECT_NE(bstring{L"ABC"}, L"abc"_bstr);
}

TEST(BinaryString_UT, equality_WhenComparedToStdStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(std::wstring{L"abc"}, bstring{L"abc"});
	EXPECT_EQ(bstring{L"abc"}, std::wstring{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(std::wstring{L"abc"}, bstring{L"ABC"});
	EXPECT_NE(bstring{L""}, std::wstring{L"abc"});
}

TEST(BinaryString_UT, equality_WhenComparedToWStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(bstring{L"abc"}, wstring{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(bstring{L"abc"}, wstring{L""});
	EXPECT_NE(bstring{L"ABC"}, wstring{L"abc"});
}

TEST(BinaryString_UT, equality_WhenComparedToStdStringViews) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(std::wstring_view{L"abc"}, bstring{L"abc"});
	EXPECT_EQ(bstring{L"abc"}, std::wstring_view{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(std::wstring_view{L"AB"}, bstring{L"abc"});
	EXPECT_NE(bstring{L"abc"}, std::wstring_view{L""});
}

TEST(BinaryString_UT, equality_WhenEmptyBStr) 
{	
	//! @test  Verify result is equal when both strings are @e empty @c BSTR
	EXPECT_EQ(bstring{L""}, bstring{L""});
}

TEST(BinaryString_UT, equality_WhenNullBStr) 
{	
	//! @test  Verify result is equal when both strings are @e null @c BSTR
	EXPECT_EQ(bstring{}, bstring{});
	EXPECT_EQ(bstring{(wchar_t const*)nullptr}, bstring{});
	EXPECT_EQ(bstring{}, bstring{(wchar_t const*)nullptr});
}

TEST(BinaryString_UT, equality_BetweenNullBStrAndEmptyBStr) 
{	
	//! @test  Verify result is @e equality between @e null and @e empty @c BSTR
	EXPECT_EQ(bstring{L""}, bstring{});
	EXPECT_EQ(bstring{}, bstring{L""});
	EXPECT_EQ(bstring{L""}, bstring{(wchar_t const*)nullptr});
	EXPECT_EQ(bstring{(wchar_t const*)nullptr}, bstring{L""});

	//! @remarks  This behaviour is consistent with Microsoft's @c BSTR wrapper @c _bstr_t
}

TEST(BinaryString_UT, Empty_TrueWhenNullBStr) 
{
	//! @test  Verify result is @c true when a @e null @c BSTR
	EXPECT_TRUE(bstring{}.empty());
	EXPECT_TRUE(bstring{(wchar_t const*)nullptr}.empty());
}

TEST(BinaryString_UT, Empty_TrueWhenEmptyBStr) 
{
	std::wstring const src{L"abc"};

	//! @test  Verify result is @c true when an @e empty @c BSTR
	EXPECT_TRUE(bstring{L""}.empty());
	EXPECT_TRUE(bstring(src.begin(), src.begin()).empty());
}

TEST(BinaryString_UT, Empty_FalseWhenNonEmpty) 
{
	//! @test  Verify result is @c false
	EXPECT_FALSE(bstring{L"abc"}.empty());
}

TEST(BinaryString_UT, Front_ReturnsFirstCharacter) 
{
	//! @test  Verify result is first character
	EXPECT_EQ(L'a', bstring{L"abc"}.front());
}

TEST(BinaryString_UT, Front_ThrowsWhenEmpty) 
{
	//! @test  Verify behaviour when string is @e empty @c BSTR
	EXPECT_ANY_THROW(bstring{L""}.front());
}

TEST(BinaryString_UT, Front_ThrowsWhenNull) 
{
	//! @test  Verify behaviour when string is a @e null @c BSTR
	EXPECT_ANY_THROW(bstring{}.front());
	EXPECT_ANY_THROW(bstring{(wchar_t const*)nullptr}.front());
}

TEST(BinaryString_UT, Size_ReturnsNumberOfCharsWhenNonEmpty) 
{
	bstring src{L"abc"};

	//! @test  Verify result is in characters, not bytes
	EXPECT_EQ(3u, src.size());
}

TEST(BinaryString_UT, Size_ReturnsZeroWhenEmpty) 
{
	//! @test  Verify result is zero when empty
	EXPECT_EQ(0u, bstring{L""}.size());
}

TEST(BinaryString_UT, Size_ReturnsZeroWhenNull) 
{
	//! @test  Verify result is zero when a @e null @c BSTR
	EXPECT_EQ(0u, bstring{}.size());
	EXPECT_EQ(0u, bstring{(wchar_t const*)nullptr}.size());
}

} // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#endif	// DISABLE_BINARY_STRING_UNIT_TESTS