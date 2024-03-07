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
//#define DISABLE_VARIANT_UNIT_TESTS
#ifndef DISABLE_VARIANT_UNIT_TESTS
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include <gtest/gtest.h>
#include "com/Variant.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
using namespace core;
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

TEST(Variant_UT, DefaultConstructor_InitiallyEmpty) 
{
	//! @test  Verify default-constructed instance is VT_EMPTY
	EXPECT_TRUE(
		com::variant{}.empty()
	);
}

TEST(Variant_UT, NullptrConstructor_InitiallyNull) 
{
	//! @test  Verify constructing with @c nullptr creates VT_NULL
	EXPECT_EQ(
		VT_NULL, com::variant{nullptr}.kind()
	);
}

TEST(Variant_UT, CopyConstructor_ClonesIntegers) 
{
	com::variant const r{42}, s{r};
	
	//! @test  Verify value was copied
	EXPECT_EQ(42, (int)s);

	//! @test  Verify runtime type was copied
	EXPECT_EQ(VT_INT, s.kind());
}

TEST(Variant_UT, CopyConstructor_ClonesStrings)
{
	com::variant const r{L"abc"}, s{r};
	
	//! @test  Verify value was copied
	EXPECT_EQ(L"abc", (com::wstring)s);
	
	//! @test  Verify runtime type was copied
	EXPECT_EQ(VT_BSTR, s.kind());
}

TEST(Variant_UT, CopyConstructor_BumpsIUnknownRefCount)
{
	FAIL();
}

TEST(Variant_UT, CopyConstructor_BumpsIDispatchRefCount)
{
	FAIL();
}

TEST(Variant_UT, MoveConstructor_TransfersInput)
{
	com::variant r{L"abc"}, s{std::move(r)};
	
	//! @test  Verify value was moved
	EXPECT_EQ(L"abc", (com::wstring)s);
	EXPECT_EQ(com::variant{}, r);
	
	//! @test  Verify runtime type was transferred
	EXPECT_EQ(VT_BSTR, s.kind());
	EXPECT_EQ(VT_EMPTY, r.kind());
}

TEST(Variant_UT, MoveConstructor_MaintainsIUnknownRefCount)
{
	FAIL();
}

TEST(Variant_UT, MoveConstructor_MaintainsIDispatchRefCount)
{
	FAIL();
}

TEST(Variant_UT, empty_TrueWhenEmpty) 
{
	//! @test  Verify @c empty() detects VT_EMPTY
	EXPECT_TRUE(com::variant{}.empty());
}

TEST(Variant_UT, empty_TrueWhenNull)
{
	//! @test  Verify @c empty() detects VT_NULL
	EXPECT_TRUE(com::variant{nullptr}.empty());
}

TEST(Variant_UT, empty_FalseWhenNonEmpty) 
{
	// Enumerate all non-null, non-empty, non-property runtime types
	for (auto e : core::enumerator_dictionary_v<::VARENUM>) 
	{
		if (e.second < VT_I2 || e.second > VT_VERSIONED_STREAM)
			continue;

		//! @test  Verify empty detects everything else
		VARIANT r{static_cast<VARTYPE>(e.second)};
		EXPECT_FALSE(com::variant{r}.empty());
	}
}

TEST(Variant_UT, kind_CorrectWhenEmpty) 
{
	//! @test  Verify @e empty variants are @c VT_EMPTY
	EXPECT_EQ(VT_EMPTY, com::variant{}.kind());
}

TEST(Variant_UT, kind_CorrectWhenNull) 
{
	//! @test  Verify @e null variants are @c VT_NULL
	EXPECT_EQ(VT_NULL, com::variant{nullptr}.kind());
}

TEST(Variant_UT, kind_CorrectWhenShort) 
{
	//! @test  Verify @c short variants are @c VT_I2
	EXPECT_EQ(VT_I2, com::variant{(short)42}.kind());
}

TEST(Variant_UT, kind_CorrectWhenUnsignedShort) 
{
	//! @test  Verify <tt>unsigned short</tt> variants are @c VT_UI2
	EXPECT_EQ(VT_UI2, com::variant{(unsigned short)42u}.kind());
}

TEST(Variant_UT, kind_CorrectWhenInt)
{
	//! @test  Verify @c int variants are @c VT_INT
	EXPECT_EQ(VT_INT, com::variant{42}.kind());
}

TEST(Variant_UT, kind_CorrectWhenUnsignedInt)
{
	//! @test  Verify @c unsigned variants are @c VT_UINT
	EXPECT_EQ(VT_UINT, com::variant{42u}.kind());
}

TEST(Variant_UT, kind_CorrectWhenLong)
{
	//! @test  Verify @c long variants are @c VT_I4
	EXPECT_EQ(VT_I4, com::variant{42l}.kind());
}

TEST(Variant_UT, kind_CorrectWhenUnsignedLong)
{
	//! @test  Verify <tt>unsigned long</tt> variants are @c VT_UI4
	EXPECT_EQ(VT_UI4, com::variant{42ul}.kind());
}

TEST(Variant_UT, kind_CorrectWhenLongLong)
{
	//! @test  Verify <tt>long long</tt> variants are @c VT_I8
	EXPECT_EQ(VT_I8, com::variant{42ll}.kind());
}

TEST(Variant_UT, kind_CorrectWhenUnsignedLongLong)
{
	//! @test  Verify <tt>unsigned long long</tt> variants are @c VT_UI8
	EXPECT_EQ(VT_UI8, com::variant{42ull}.kind());
}

TEST(Variant_UT, kind_CorrectWhenFloat) 
{
	//! @test  Verify @c float variants are @c VT_R4
	EXPECT_EQ(VT_R4, com::variant{42.0f}.kind());
}

TEST(Variant_UT, kind_CorrectWhenDouble) 
{
	//! @test  Verify @c double variants are @c VT_R8
	EXPECT_EQ(VT_R8, com::variant{42.0}.kind());
}

TEST(Variant_UT, kind_CorrectWhenString)
{
	//! @test  Verify @e string variants are @c VT_BSTR
	EXPECT_EQ(VT_BSTR, com::variant{"abc"}.kind());
}

TEST(Variant_UT, kind_CorrectWhenHResult) 
{
	//! @test  Verify @c win::HResult variants are @c VT_ERROR
	EXPECT_EQ(VT_ERROR, com::variant{win::HResult{E_FAIL}}.kind());
}

TEST(Variant_UT, kind_CorrectWhenIUnknown) 
{
	//! @test  Verify @c IUnknown variants are @c VT_UNKNOWN
	EXPECT_EQ(VT_UNKNOWN, com::variant{(IUnknown*)nullptr}.kind());
}

TEST(Variant_UT, kind_CorrectWhenIDispatch) 
{
	//! @test  Verify @c IDispatch variants are @c VT_DISPATCH
	EXPECT_EQ(VT_DISPATCH, com::variant{(IDispatch*)nullptr}.kind());
}

TEST(Variant_UT, str_CorrectWhenEmpty) 
{
	//! @test  Verify @e empty variants return fixed string
	EXPECT_EQ("Empty", com::variant{}.str());
}

TEST(Variant_UT, str_CorrectWhenNull) 
{
	//! @test  Verify @e null variants return fixed string
	EXPECT_EQ("Null", com::variant{nullptr}.str());
}

TEST(Variant_UT, str_CorrectWhenChar) 
{
	//! @test  Verify @c char variants return string-representation of their value
	EXPECT_EQ("42", com::variant{(char)42}.str());
}

TEST(Variant_UT, str_CorrectWhenByte) 
{
	//! @test  Verify <tt>unsigned char</tt> variants return string-representation of their value
	EXPECT_EQ("42", com::variant{(unsigned char)42}.str());
}

TEST(Variant_UT, str_CorrectWhenShort) 
{
	//! @test  Verify @c short variants return string-representation of their value
	EXPECT_EQ("42", com::variant{(short)42}.str());
}

TEST(Variant_UT, str_CorrectWhenUnsignedShort) 
{
	//! @test  Verify <tt>unsigned short</tt> variants return string-representation of their value
	EXPECT_EQ("42", com::variant{(unsigned short)42}.str());
}

TEST(Variant_UT, str_CorrectWhenInt) 
{
	//! @test  Verify @c int variants return string-representation of their value
	EXPECT_EQ("42", com::variant{42}.str());
}

TEST(Variant_UT, str_CorrectWhenUnsignedInt) 
{
	//! @test  Verify <tt>unsigned short</tt> variants return string-representation of their value
	EXPECT_EQ("42", com::variant{42u}.str());
}

TEST(Variant_UT, str_CorrectWhenLong)
{
	//! @test  Verify @c long variants return string-representation of their value
	EXPECT_EQ("42", com::variant{42l}.str());
}

TEST(Variant_UT, str_CorrectWhenUnsignedLong) 
{
	//! @test  Verify <tt>unsigned long</tt> variants return string-representation of their value
	EXPECT_EQ("42", com::variant{42ul}.str());
}

TEST(Variant_UT, str_CorrectWhenLongLong)
{
	//! @test  Verify <tt>long long</tt> variants return string-representation of their value
	EXPECT_EQ("42", com::variant{42ll}.str());
}

TEST(Variant_UT, str_CorrectWhenUnsignedLongLong) 
{
	//! @test  Verify <tt>unsigned long long</tt> variants return string-representation of their value
	EXPECT_EQ("42", com::variant{42ull}.str());
}

TEST(Variant_UT, str_CorrectWhenFloat)
{
	//! @test  Verify @c float variants return string-representation of their value
	EXPECT_EQ("42.1", com::variant{42.1f}.str());
}

TEST(Variant_UT, str_CorrectWhenDouble)
{
	//! @test  Verify @c double variants return string-representation of their value
	EXPECT_EQ("42.1", com::variant{42.1}.str());
}

TEST(Variant_UT, str_CorrectWhenDecimal)
{
	FAIL();
}

TEST(Variant_UT, str_CorrectWhenString)
{
	//! @test  Verify narrow-string variants return their value
	EXPECT_EQ("str", com::variant{"str"}.str());
	
	//! @test  Verify wide-string variants return their value
	EXPECT_EQ("str", com::variant{L"str"}.str());
}

TEST(Variant_UT, str_CorrectWhenDate)
{
	FAIL();
}

TEST(Variant_UT, str_CorrectWhenHResult)
{
	FAIL();
}

TEST(Variant_UT, str_CorrectWhenCurrency)
{
	FAIL();
}

TEST(Variant_UT, str_CorrectWhenIUnknown)
{
	FAIL();
}

TEST(Variant_UT, str_CorrectWhenIDispatch)
{
	FAIL();
}

TEST(Variant_UT, wstr_CorrectWideStringRepresentation) 
{	
	//! @test  Verify string variants return their value
	EXPECT_EQ(L"wstr", com::variant{L"wstr"}.wstr());
}

TEST(Variant_UT, valid_FalseForInvalidRuntimeType) 
{
	//! @test  Verify variants with unrecognised runtime type are @e invalid
	EXPECT_FALSE(com::variant::valid(static_cast<::VARENUM>(VT_VERSIONED_STREAM + 1)));
}

TEST(Variant_UT, valid_TrueForValidRuntimeType) 
{
	//! @test  Verify variants with recognised runtime type are @e valid
	EXPECT_TRUE(com::variant::valid(VT_R4));
}

TEST(Variant_UT, assignment_ValueChangedWhenChar)
{
	com::variant obj{(char)42};
	obj = (char)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (char)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotChar)
{
	com::variant obj{42};
	obj = (char)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (char)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_I1, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenByte)
{
	com::variant obj{(unsigned char)42};
	obj = (unsigned char)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (unsigned char)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotByte)
{
	com::variant obj{42};
	obj = (unsigned char)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (unsigned char)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UI1, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenShort)
{
	com::variant obj{(short)42};
	obj = (short)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (short)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotShort)
{
	com::variant obj{42};
	obj = (short)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (short)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_I2, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenUnsignedShort)
{
	com::variant obj{(unsigned short)42};
	obj = (unsigned short)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (unsigned short)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotUnsignedShort)
{
	com::variant obj{42};
	obj = (unsigned short)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (unsigned short)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UI2, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenInt)
{
	com::variant obj{42};
	obj = 43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (int)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotInt)
{
	com::variant obj{true};
	obj = 43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (int)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_INT, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenUnsignedInt)
{
	com::variant obj{42u};
	obj = 43u;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43u, (unsigned)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotUnsignedInt)
{
	com::variant obj{false};
	obj = (unsigned)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43u, (unsigned)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UINT, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenLong)
{
	com::variant obj{42l};
	obj = 43l;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43l, (long)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotLong)
{
	com::variant obj{(short)22};
	obj = 43l;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43l, (long)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_I4, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenUnsignedLong)
{
	com::variant obj{42ul};
	obj = 43ul;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ul, (unsigned long)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotUnsignedLong)
{
	com::variant obj{"42"};
	obj = 43ul;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ul, (unsigned long)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UI4, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenLongLong)
{
	com::variant obj{42ll};
	obj = 43ll;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ll, (long long)obj);
}

#if (_WIN32_WINNT >= 0x0501)
TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotLongLong)
{
	com::variant obj{42.0f};
	obj = 43ll;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ll, (long long)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_I8, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenUnsignedLongLong)
{
	com::variant obj{42ull};
	obj = 43ull;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ull, (unsigned long long)obj);
}
#endif

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotUnsignedLongLong)
{
	com::variant obj{(short)42};
	obj = 43ull;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ull, (unsigned long long)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UI8, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenFloat)
{
	com::variant obj{42.2};
	obj = 43.3;

	//! @test  Verify assignment changed value
	EXPECT_FLOAT_EQ(43.3f, (float)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotFloar)
{
	com::variant obj{42};
	obj = 43.0f;

	//! @test  Verify assignment changed value
	EXPECT_FLOAT_EQ(43.0f, (float)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_R4, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenDouble)
{
	com::variant obj{42.0};
	obj = 43.0;

	//! @test  Verify assignment changed value
	EXPECT_DOUBLE_EQ(43.0, (double)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotDouble)
{
	com::variant obj{win::HResult{S_OK}};
	obj = 43.0;

	//! @test  Verify assignment changed value
	EXPECT_DOUBLE_EQ(43.0, obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_R8, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenString)
{
	com::variant obj{"abc"};
	obj = "ac";

	//! @test  Verify assignment changed value
	EXPECT_EQ(L"ac", (com::wstring)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotString)
{
	com::variant obj{42.0};
	obj = "43";

	//! @test  Verify assignment changed value
	EXPECT_EQ(L"43", (com::wstring)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_BSTR, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenDate)
{
	FAIL();
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotDate)
{
	FAIL();
}

TEST(Variant_UT, assignment_ValueChangedWhenDecimal)
{
	FAIL();
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotDecimal)
{
	FAIL();
}

TEST(Variant_UT, assignment_ValueChangedWhenCurrency)
{
	FAIL();
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotCurrency)
{
	FAIL();
}

TEST(Variant_UT, assignment_ValueChangedWhenHResult)
{
	com::variant obj{win::HResult{E_FAIL}};
	obj = win::HResult{S_OK};

	//! @test  Verify assignment changed value
	EXPECT_EQ(S_OK, (win::HResult)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotHResult)
{
	com::variant obj{(unsigned char)42};
	obj = win::HResult{E_OUTOFMEMORY};

	//! @test  Verify assignment changed value
	EXPECT_EQ(E_OUTOFMEMORY, (win::HResult)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_ERROR, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenIUnknown)
{
	FAIL();
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotIUnknown)
{
	FAIL();
}

TEST(Variant_UT, assignment_ValueChangedWhenIDispatch)
{
	FAIL();
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotIDispatch)
{
	FAIL();
}

TEST(Variant_UT, conversion_IntegerConversionsSucceedWhenValueCanBeRepresented) 
{
	//! @test  Verify integer conversions succeed
	EXPECT_EQ((char)42, (char)com::variant{42});
}

TEST(Variant_UT, conversion_IntegerPromotionsSucceedWhenValueCanBeRepresented) 
{
	//! @test  Verify integer promotions succeed
	EXPECT_EQ(42l, (long)com::variant{42});
}

TEST(Variant_UT, conversion_FloatingPointConversionsSucceedWhenValueCanBeRepresented) 
{
	//! @test  Verify floating-point conversions succeed
	EXPECT_FLOAT_EQ(42.1f, (float)com::variant{42.1});
}

TEST(Variant_UT, conversion_FloatingPointPromotionsSucceedWhenValueCanBeRepresented) 
{
	//! @test  Verify floating-point promotions succeed
	EXPECT_DOUBLE_EQ(42.1, (double)com::variant{42.1f});
}

TEST(Variant_UT, conversion_IntegerToStringConversionsSucceedsWhenValueCanBeRepresented) 
{
	//! @test  Verify integer-to-string conversions succeed
	EXPECT_EQ(L"42", (com::BinaryString)com::variant{42});
}

TEST(Variant_UT, conversion_StringToIntegerSucceedsWhenValueCanBeRepresented) 
{
	//! @test  Verify string-to-integer conversions succeed
	EXPECT_EQ(42, (int)com::variant{L"42"});
}

TEST(Variant_UT, conversion_FailsWhenValueCannotBeRepresented) 
{
	//! @test  Verify conversions fail when overflow would occur
	EXPECT_THROW((short)com::variant{INT_MAX}, range_error);
}

TEST(Variant_UT, conversion_FailsWhenConversionIsIllegal) 
{
	//! @test  Verify illegal type conversions fail
	EXPECT_THROW((::IUnknown*)com::variant{42}, logic_error);
}

TEST(Variant_UT, conversion_FailsWhenRuntimeTypeIsInvalid) 
{
	::VARIANT const Invalid{VT_VERSIONED_STREAM + 1};
	
	//! @test  Verify conversions fails when runtime type is invalid
	EXPECT_ANY_THROW((int)com::variant{Invalid});
}

TEST(Variant_UT, roundtrip_BoolValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(true, (bool)com::variant{true});
}

TEST(Variant_UT, roundtrip_CharValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ((char)42, (char)com::variant{42});
}

TEST(Variant_UT, roundtrip_UnsignedCharValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ((unsigned char)42, (unsigned char)com::variant{42});
}

TEST(Variant_UT, roundtrip_ShortValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ((short)42, (short)com::variant{42});
}

TEST(Variant_UT, roundtrip_UnsignedShortValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ((unsigned short)42, (unsigned short)com::variant{42});
}

TEST(Variant_UT, roundtrip_IntValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(42, (int)com::variant{42});
}

TEST(Variant_UT, roundtrip_UnsignedIntValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(42u, (unsigned)com::variant{42u});
}

TEST(Variant_UT, roundtrip_LongValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(42l, (int)com::variant{42l});
}

TEST(Variant_UT, roundtrip_UnsignedLongValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(42ul, (unsigned)com::variant{42ul});
}

#if (_WIN32_WINNT >= 0x0501)
	TEST(Variant_UT, roundtrip_LongLongValueIsUnchanged) 
	{
		//! @test  Verify value is unmodified
		EXPECT_EQ(42ll, (int)com::variant{42ll});
	}

	TEST(Variant_UT, roundtrip_UnsignedLongLongValueIsUnchanged) 
	{
		//! @test  Verify value is unmodified
		EXPECT_EQ(42ull, (unsigned)com::variant{42ull});
	}
#endif

TEST(Variant_UT, roundtrip_FloatValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_FLOAT_EQ(42.2f, (float)com::variant{42.2});
}

TEST(Variant_UT, roundtrip_DoubleValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_DOUBLE_EQ(42.2, (double)com::variant{42.2});
}

TEST(Variant_UT, roundtrip_DecimalValueIsUnchanged) 
{
	FAIL();
}

TEST(Variant_UT, roundtrip_CurrencyValueIsUnchanged) 
{
	FAIL();
}

TEST(Variant_UT, roundtrip_DateValueIsUnchanged) 
{
	FAIL();
}

TEST(Variant_UT, roundtrip_NarrowStringValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(L"abc", (com::wstring)com::variant{"abc"});
}

TEST(Variant_UT, roundtrip_WideStringValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(L"wabc", (com::wstring)com::variant{L"wabc"});
}

TEST(Variant_UT, roundtrip_IUnknownValueIsUnchanged) 
{
	FAIL();
}

TEST(Variant_UT, roundtrip_IDispatchValueIsUnchanged) 
{
	FAIL();
}

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#endif	// DISABLE_VARIANT_UNIT_TESTS