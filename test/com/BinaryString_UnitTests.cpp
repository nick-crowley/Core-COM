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
TEST(BinaryString_UT, Constructor_Succeeds) 
{
	//! @test  Verify string can be constructed
	EXPECT_NO_THROW(
		wstring{L"hello"}
	);
}

TEST(BinaryString_UT, CopyConstructor_ClonesInput) 
{
	wstring const src{L"abc"};
	
	//! @test  Verify characters were copied
	EXPECT_TRUE(
		wstring{src} == L"abc"
	);
}

TEST(BinaryString_UT, MoveConstructor_TransfersInput) 
{
	wstring src{L"abc"};
	
	//! @test  Verify characters were moved
	EXPECT_TRUE(
		wstring{std::move(src)} == L"abc"
	);

	//! @post  Input string is empty
	EXPECT_TRUE(
		src.empty()
	);
}

TEST(BinaryString_UT, IteratorPairConstructor_ClonesInputRange) 
{
	std::wstring const src{L"abc"};
	
	//! @test  Verify characters of source range were copied
	EXPECT_TRUE(
		wstring(src.begin(), src.end()) == L"abc"
	);
}

TEST(BinaryString_UT, AdoptConstructor_TakesOwnershipOfInputBuffer) 
{
	wchar_t* buffer = ::SysAllocString(L"abc");
	
	//! @test  Verify buffer has been transferred, not copied, into string
	EXPECT_EQ(
		(void*)wstring(adopt, buffer).c_str(),
		(void*)buffer
	);
}

TEST(BinaryString_UT, Assign_ClonesInputRange) 
{
	std::wstring const src{L"abc"};

	//! @test  Verify characters of source range were assigned
	wstring obj{};
	obj.assign(src.begin(), src.end());
	EXPECT_TRUE(obj == L"abc");
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
} // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#endif	// DISABLE_BINARY_STRING_UNIT_TESTS