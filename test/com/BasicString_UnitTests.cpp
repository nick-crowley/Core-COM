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
//#define DISABLE_BASIC_STRING_UNIT_TESTS
#ifndef DISABLE_BASIC_STRING_UNIT_TESTS
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include <gtest/gtest.h>
#include "com/BasicString.h"
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
TEST(BasicString_UT, DefaultConstructor_ConstructsNullBSTR) 
{
	bstr obj;

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is @e null @c BSTR rather than @e empty @c BSTR
	EXPECT_EQ(nullptr, obj.c_str());
	EXPECT_EQ(nullptr, obj.data());
}

TEST(BasicString_UT, CopyConstructor_ClonesConstexprBString) 
{	
	//! @test  Verify characters are copied from string with different allocator
	EXPECT_EQ(L"abc", bstr{L"abc"_bstr});
}

TEST(BasicString_UT, CopyConstructor_ClonesNonEmptyString) 
{
	bstr const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", bstr{src});
}

TEST(BasicString_UT, CopyConstructor_ClonesEmptyBStr) 
{
	bstr const src{L""};
	
	//! @test  Verify result is @e empty @c BSTR
	EXPECT_EQ(L"", bstr{src});
}

TEST(BasicString_UT, CopyConstructor_ClonesNullBStr) 
{
	bstr const src;
	
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, bstr{src}.data());
}

TEST(BasicString_UT, CopyConstructor_ClonesWString) 
{	
	//! @test  Verify characters are copied from string with different allocator
	EXPECT_EQ(L"abc", bstr{wstring{L"abc"}});
}

TEST(BasicString_UT, MoveConstructor_TransfersContentLeavingNullBStr) 
{
	bstr src{L"abc"};
	
	//! @test  Verify characters were moved
	EXPECT_EQ(L"abc", bstr{std::move(src)});

	//! @post  Input string is now @e null @c BSTR
	EXPECT_TRUE(src.empty());
	EXPECT_EQ(nullptr, src.data());
}

TEST(BasicString_UT, IteratorPairConstructor_ClonesInputRange) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify characters of source range were copied
	EXPECT_EQ(L"abc", bstr(src.begin(), src.end()));
}

TEST(BasicString_UT, IteratorPairConstructor_ConvertsInputRange) 
{
	std::string const src{"abc"};
	
	//! @test  Verify characters of source range were converted and copied
	EXPECT_EQ(L"abc", bstr(src.begin(), src.end()));
}

TEST(BasicString_UT, IteratorPairConstructor_RejectsInvalidRange) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify invalid argument is rejected
	EXPECT_ANY_THROW(
		bstr(src.end(), src.begin())
	);
}

TEST(BasicString_UT, IteratorPairConstructor_RejectsMissingArgument) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify either missing argument is detected
	EXPECT_ANY_THROW(bstr(&src[0], (wchar_t const*)nullptr));
	EXPECT_ANY_THROW(bstr((wchar_t const*)nullptr, &src[1]));
}

TEST(BasicString_UT, IteratorPairConstructor_EmptyStringWhenRangeIsEmpty) 
{
	std::wstring const src{L"abc"};
	bstr obj{src.begin(), src.begin()};

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is an @e empty @c BSTR rather than @e null @c BSTR
	EXPECT_NE(nullptr, obj.c_str());
	EXPECT_NE(nullptr, obj.data());
	EXPECT_STREQ(L"", obj.c_str());
}

TEST(BasicString_UT, IteratorPairConstructor_NullStringWhenRangeIsBothNull) 
{
	std::wstring const src{L"abc"};
	bstr obj{(wchar_t const*)nullptr, (wchar_t const*)nullptr};

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is an @e null @c BSTR rather than @e empty @c BSTR
	EXPECT_EQ(nullptr, obj.c_str());
	EXPECT_EQ(nullptr, obj.data());
}

TEST(BasicString_UT, AdoptConstructor_TakesOwnershipOfInputBuffer) 
{
	wchar_t* buffer = ::SysAllocString(L"abc");
	
	//! @test  Verify buffer has been transferred, not copied, into string
	EXPECT_EQ(
		(void*)buffer,
		(void*)bstr(adopt, buffer).c_str()
	);
}

TEST(BasicString_UT, AdoptConstructor_ConstructsEmptyBStr) 
{
	wchar_t* buffer = ::SysAllocString(L"");
	
	//! @test  Verify buffer is empty
	EXPECT_EQ(0u, wcslen(buffer));
	
	//! @test  Verify buffer has been transferred, not copied, into string
	EXPECT_EQ(
		(void*)buffer,
		(void*)bstr(adopt, buffer).c_str()		
	);
}

TEST(BasicString_UT, AdoptConstructor_ConstructsNullBStr) 
{
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(
		nullptr,
		bstr(adopt, nullptr).c_str()
	);
}

TEST(BasicString_UT, CStringConstructor_ClonesInputBuffer) 
{
	auto const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", bstr{src});
	
	//! @test  Verify buffer has been copied, not transferred, into string
	EXPECT_NE(
		(void*)src,
		(void*)bstr(src).c_str()		
	);
}

TEST(BasicString_UT, CStringConstructor_ConstructsEmptyBStr) 
{
	//! @test  Verify result is @e empty @c BSTR
	EXPECT_NE(nullptr, bstr(L"").c_str());
}

TEST(BasicString_UT, CStringConstructor_ConstructsNullBStr) 
{
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, bstr((const wchar_t*)nullptr).c_str());
}

TEST(BasicString_UT, StdStringConstructor_ClonesNonEmptyNarrowString) 
{
	std::string const src{"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", bstr{src});
}

TEST(BasicString_UT, StdStringConstructor_ClonesNonEmptyWideString) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"abc", bstr{src});
}

TEST(BasicString_UT, CopyAssignment_ClonesConstexprBString) 
{
	bstr dest{L"abc"};
	noopstring const lvalue{L"def"_bstr};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"def", dest = lvalue);
}

TEST(BasicString_UT, CopyAssignment_ClonesConstexprBStringRValue) 
{
	bstr dest{L"abc"};

	//! @test  Verify characters were copied
	EXPECT_EQ(L"def", dest = L"def"_bstr);
	EXPECT_FALSE(dest.empty());
}

TEST(BasicString_UT, CopyAssignment_ClonesNonEmptyString) 
{
	bstr dest{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"def", dest = L"def");
}

TEST(BasicString_UT, CopyAssignment_ClonesEmptyBStr) 
{
	bstr dest{L"abc"};
	
	//! @test  Verify result is @e empty @c BSTR
	EXPECT_EQ(L"", dest = L"");
}

TEST(BasicString_UT, CopyAssignment_ClonesNullBStr) 
{
	bstr dest{L"abc"};
	dest = bstr{};
	
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, dest.data());
}

TEST(BasicString_UT, CopyAssignment_ClonesNullptr) 
{
	bstr dest{L"abc"};
	dest = nullptr;
	
	//! @test  Verify result is @e null @c BSTR
	EXPECT_EQ(nullptr, dest.data());
}

TEST(BasicString_UT, CopyAssignment_ClonesWString) 
{
	bstr dest{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_EQ(L"def", dest = wstring{L"def"});
}

TEST(BasicString_UT, MoveAssignment_TransfersNonEmptyString)
{
	bstr src{L"abc"};
	bstr dest{L"def"};
	
	//! @test  Verify characters are present in destination
	EXPECT_EQ(L"abc", dest = std::move(src));

	//! @test  Verify content was moved rather than copied
	EXPECT_TRUE(src.empty());
}

TEST(BasicString_UT, Assign_ClonesInputRange) 
{
	std::wstring const src{L"abc"};

	//! @test  Verify characters of source range were assigned
	bstr obj{};
	obj.assign(src.begin(), src.end());
	EXPECT_EQ(obj, L"abc");
}

TEST(BasicString_UT, Assign_EmptyBStrWhenRangeIsEmpty) 
{
	std::wstring const src{L""};
	bstr obj{};
	obj.assign(src.begin(), src.end());

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is @e empty @c BSTR rather than @e null @c BSTR
	EXPECT_NE(nullptr, obj.data());
	EXPECT_NE(nullptr, obj.c_str());
	EXPECT_STREQ(L"", obj.c_str());
}

TEST(BasicString_UT, Assign_NullBStrWhenRangeIsNull) 
{
	bstr obj{L"abc"};
	obj.assign((wchar_t const*)nullptr, (wchar_t const*)nullptr);

	//! @test  Verify object is @e empty
	EXPECT_TRUE(obj.empty());
	EXPECT_EQ(0u, obj.size());

	//! @test  Verify result is @e null @c BSTR rather than @e empty @c BSTR
	EXPECT_EQ(nullptr, obj.data());
	EXPECT_EQ(nullptr, obj.c_str());
}

TEST(BasicString_UT, Assign_RejectsMissingArgument) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify either missing argument is detected
	EXPECT_ANY_THROW(bstr{}.assign(&src[0], (wchar_t const*)nullptr));
	EXPECT_ANY_THROW(bstr{}.assign((wchar_t const*)nullptr, &src[1]));
}

TEST(BasicString_UT, Back_ReturnsLastCharacter) 
{
	//! @test  Verify result is final character
	EXPECT_EQ(L'c', bstr{L"abc"}.back());
}

TEST(BasicString_UT, Back_ThrowsWhenEmpty) 
{
	//! @test  Verify behaviour when string is @e empty @c BSTR
	EXPECT_ANY_THROW(bstr{L""}.back());
}

TEST(BasicString_UT, Back_ThrowsWhenNull) 
{
	//! @test  Verify behaviour when string is @e null @c BSTR
	EXPECT_ANY_THROW(bstr{}.back());
	EXPECT_ANY_THROW(bstr{(wchar_t const*)nullptr}.back());
}

TEST(BasicString_UT, CStr_ReturnsNullTerminatedContentWhenNonEmpty) 
{
	//! @test  Verify result is accurate and null-terminated
	EXPECT_STREQ(L"abc", bstr{L"abc"}.c_str());
}

TEST(BasicString_UT, CStr_ReturnsEmptyNullTerminatedBufferWhenEmptyBStr) 
{
	//! @test  Verify result is not @c nullptr when @e empty
	EXPECT_STREQ(L"", bstr{L""}.c_str());
	
	//! @test  Verify result is not a copy but the real buffer
	bstr src{L"abc"};
	EXPECT_EQ(src.data(), src.c_str());
}

TEST(BasicString_UT, CStr_ReturnsNullptrWhenNullBStr)
{
	//! @test  Verify result is @c nullptr when a @e null @c BSTR
	EXPECT_EQ(nullptr, bstr{}.c_str());
	EXPECT_EQ(nullptr, bstr{(wchar_t const*)nullptr}.c_str());
}

TEST(BasicString_UT, Data_CompatibleWithSysFreeString) 
{
	bstr src{L"abc"};

	//! @test  Verify @c SysFreeString() can free the detached BSTR
	EXPECT_NO_FATAL_FAILURE(::SysFreeString(src.detach()));
}

TEST(BasicString_UT, Data_CompatibleWithSysStringLen) 
{
	bstr src{L"abc"};

	//! @test  Verify @c SysStringLen() can obtain the length from the BSTR
	EXPECT_EQ(3u, ::SysStringLen(src.bstr()));
}

TEST(BasicString_UT, Data_ReturnsNullptrWhenNullBStr) 
{
	//! @test  Verify result is @c nullptr when a @e null @c BSTR
	EXPECT_EQ(nullptr, bstr{}.data());
	EXPECT_EQ(nullptr, bstr{(wchar_t const*)nullptr}.data());
}

TEST(BasicString_UT, Data_ReturnsNonNullWhenEmptyBStr) 
{
	//! @test  Verify result is not @c nullptr when an @e empty @c BSTR
	EXPECT_NE(nullptr, bstr{L""}.data());
}

TEST(BasicString_UT, Detach_NullWhenEmpty) 
{
	//! @test  Verify result is nullptr when empty
	EXPECT_EQ(nullptr, bstr{}.detach());
}

TEST(BasicString_UT, Detach_ObjectIsEmptyAfterwards) 
{
	bstr src{L"abc"};
	src.detach();

	//! @test  Verify object is @e empty afterwards
	EXPECT_TRUE(src.empty());
}

TEST(BasicString_UT, Detach_ReturnsNullTerminatedBuffer) 
{
	//! @test  Verify buffer is complete and null-terminated
	EXPECT_STREQ(L"abc", bstr{L"abc"}.detach());
}

TEST(BasicString_UT, equality_WhenComparedToCStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(L"abc", bstr{L"abc"});
	EXPECT_EQ(bstr{L"abc"}, L"abc");

	//! @test  Verify result when unequal
	EXPECT_NE(L"abc", bstr{L"ABC"});
	EXPECT_NE(bstr{L""}, L"abc");
}

TEST(BasicString_UT, equality_WhenComparedToBStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(bstr{L"abc"}, bstr{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(bstr{L"abc"}, bstr{L""});
	EXPECT_NE(bstr{L"ABC"}, bstr{L"abc"});
}

TEST(BasicString_UT, equality_WhenComparedToConstexprBStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(bstr{L"abc"}, L"abc"_bstr);

	//! @test  Verify result when unequal
	EXPECT_NE(bstr{L"abc"}, L""_bstr);
	EXPECT_NE(bstr{L"ABC"}, L"abc"_bstr);
}

TEST(BasicString_UT, equality_WhenComparedToStdStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(std::wstring{L"abc"}, bstr{L"abc"});
	EXPECT_EQ(bstr{L"abc"}, std::wstring{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(std::wstring{L"abc"}, bstr{L"ABC"});
	EXPECT_NE(bstr{L""}, std::wstring{L"abc"});
}

TEST(BasicString_UT, equality_WhenComparedToWStrings) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(bstr{L"abc"}, wstring{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(bstr{L"abc"}, wstring{L""});
	EXPECT_NE(bstr{L"ABC"}, wstring{L"abc"});
}

TEST(BasicString_UT, equality_WhenComparedToStdStringViews) 
{	
	//! @test  Verify result when equal
	EXPECT_EQ(std::wstring_view{L"abc"}, bstr{L"abc"});
	EXPECT_EQ(bstr{L"abc"}, std::wstring_view{L"abc"});

	//! @test  Verify result when unequal
	EXPECT_NE(std::wstring_view{L"AB"}, bstr{L"abc"});
	EXPECT_NE(bstr{L"abc"}, std::wstring_view{L""});
}

TEST(BasicString_UT, equality_WhenEmptyBStr) 
{	
	//! @test  Verify result is equal when both strings are @e empty @c BSTR
	EXPECT_EQ(bstr{L""}, bstr{L""});
}

TEST(BasicString_UT, equality_WhenNullBStr) 
{	
	//! @test  Verify result is equal when both strings are @e null @c BSTR
	EXPECT_EQ(bstr{}, bstr{});
	EXPECT_EQ(bstr{(wchar_t const*)nullptr}, bstr{});
	EXPECT_EQ(bstr{}, bstr{(wchar_t const*)nullptr});
}

TEST(BasicString_UT, equality_BetweenNullBStrAndEmptyBStr) 
{	
	//! @test  Verify result is @e equality between @e null and @e empty @c BSTR
	EXPECT_EQ(bstr{L""}, bstr{});
	EXPECT_EQ(bstr{}, bstr{L""});
	EXPECT_EQ(bstr{L""}, bstr{(wchar_t const*)nullptr});
	EXPECT_EQ(bstr{(wchar_t const*)nullptr}, bstr{L""});

	//! @remarks  This behaviour is consistent with Microsoft's @c BSTR wrapper @c _bstr_t
}

TEST(BasicString_UT, Empty_TrueWhenNullBStr) 
{
	//! @test  Verify result is @c true when a @e null @c BSTR
	EXPECT_TRUE(bstr{}.empty());
	EXPECT_TRUE(bstr{(wchar_t const*)nullptr}.empty());
}

TEST(BasicString_UT, Empty_TrueWhenEmptyBStr) 
{
	std::wstring const src{L"abc"};

	//! @test  Verify result is @c true when an @e empty @c BSTR
	EXPECT_TRUE(bstr{L""}.empty());
	EXPECT_TRUE(bstr(src.begin(), src.begin()).empty());
}

TEST(BasicString_UT, Empty_FalseWhenNonEmpty) 
{
	//! @test  Verify result is @c false
	EXPECT_FALSE(bstr{L"abc"}.empty());
}

TEST(BasicString_UT, Front_ReturnsFirstCharacter) 
{
	//! @test  Verify result is first character
	EXPECT_EQ(L'a', bstr{L"abc"}.front());
}

TEST(BasicString_UT, Front_ThrowsWhenEmpty) 
{
	//! @test  Verify behaviour when string is @e empty @c BSTR
	EXPECT_ANY_THROW(bstr{L""}.front());
}

TEST(BasicString_UT, Front_ThrowsWhenNull) 
{
	//! @test  Verify behaviour when string is a @e null @c BSTR
	EXPECT_ANY_THROW(bstr{}.front());
	EXPECT_ANY_THROW(bstr{(wchar_t const*)nullptr}.front());
}

TEST(BasicString_UT, Size_ReturnsNumberOfCharsWhenNonEmpty) 
{
	bstr src{L"abc"};

	//! @test  Verify result is in characters, not bytes
	EXPECT_EQ(3u, src.size());
}

TEST(BasicString_UT, Size_ReturnsZeroWhenEmpty) 
{
	//! @test  Verify result is zero when empty
	EXPECT_EQ(0u, bstr{L""}.size());
}

TEST(BasicString_UT, Size_ReturnsZeroWhenNull) 
{
	//! @test  Verify result is zero when a @e null @c BSTR
	EXPECT_EQ(0u, bstr{}.size());
	EXPECT_EQ(0u, bstr{(wchar_t const*)nullptr}.size());
}

} // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#endif	// DISABLE_BASIC_STRING_UNIT_TESTS