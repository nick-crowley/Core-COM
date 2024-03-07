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
#include <gmock/gmock.h>
#include "com/SharedPtr.h"
#include "com/Variant.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
using namespace core;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::StrictMock;
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
struct MockComObject : ::IUnknown 
{
	MOCK_METHOD(HRESULT, QueryInterface, (REFIID riid, void** ppvObject), (Calltype(COMAPI), override));
	MOCK_METHOD(ULONG, AddRef, (), (Calltype(COMAPI), override));
	MOCK_METHOD(ULONG, Release, (), (Calltype(COMAPI), override));
};

struct MockDispatchObject : ::IDispatch
{
	MOCK_METHOD(HRESULT, QueryInterface, (REFIID riid, void** ppvObject), (Calltype(COMAPI), override));
	MOCK_METHOD(ULONG, AddRef, (), (Calltype(COMAPI), override));
	MOCK_METHOD(ULONG, Release, (), (Calltype(COMAPI), override));

	MOCK_METHOD(HRESULT, GetTypeInfoCount, (UINT *pctinfo), (Calltype(COMAPI), override));
    MOCK_METHOD(HRESULT, GetTypeInfo, (UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo), (Calltype(COMAPI), override));
    MOCK_METHOD(HRESULT, GetIDsOfNames, (REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId), (Calltype(COMAPI), override));
    MOCK_METHOD(HRESULT, Invoke, (DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr), (Calltype(COMAPI), override));
};

template <typename Mock>
std::shared_ptr<Mock>
make_mock_coclass() {
	auto obj = std::make_shared<Mock>();
	ON_CALL(*obj, QueryInterface).WillByDefault(Return(E_NOINTERFACE));
	ON_CALL(*obj, AddRef).WillByDefault(Return(1));
	ON_CALL(*obj, Release).WillByDefault(Return(1));
	return obj;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
TEST(Variant_UT, DefaultConstructor_InitiallyEmpty) 
{
	//! @test  Verify default-constructed instance is VT_EMPTY
	EXPECT_TRUE(
		variant{}.empty()
	);
}

TEST(Variant_UT, NullptrConstructor_InitiallyNull) 
{
	//! @test  Verify constructing with @c nullptr creates VT_NULL
	EXPECT_EQ(
		VT_NULL, variant{nullptr}.kind()
	);
}

TEST(Variant_UT, CopyConstructor_ClonesIntegers) 
{
	variant const r{42}, s{r};
	
	//! @test  Verify value was copied
	EXPECT_EQ(42, (int)s);

	//! @test  Verify runtime type was copied
	EXPECT_EQ(VT_INT, s.kind());
}

TEST(Variant_UT, CopyConstructor_ClonesStrings)
{
	variant const r{L"abc"}, s{r};
	
	//! @test  Verify value was copied
	EXPECT_EQ(L"abc", (wstring)s);
	
	//! @test  Verify runtime type was copied
	EXPECT_EQ(VT_BSTR, s.kind());
}

TEST(Variant_UT, CopyConstructor_BumpsRefCountWhenIUnknown)
{	
	auto obj = make_mock_coclass<StrictMock<MockComObject>>();
	
	//! @post  Copy-construction causes pair of calls to @c AddRef() and @c Release()
	EXPECT_CALL(*obj, AddRef).Times(2);
	EXPECT_CALL(*obj, Release).Times(2);

	variant const r{obj.get()}, s{r};
	
	//! @test  Verify value was copied
	EXPECT_FALSE(r.empty());
	EXPECT_FALSE(s.empty());

	//! @test  Verify runtime type was copied
	EXPECT_EQ(VT_UNKNOWN, r.kind());
	EXPECT_EQ(VT_UNKNOWN, s.kind());
}

TEST(Variant_UT, CopyConstructor_BumpsRefCountWhenIDispatch)
{
	auto obj = make_mock_coclass<StrictMock<MockDispatchObject>>();
	
	//! @post  Copy-construction causes pair of calls to @c AddRef() and @c Release()
	EXPECT_CALL(*obj, AddRef).Times(2);
	EXPECT_CALL(*obj, Release).Times(2);

	variant const r{obj.get()}, s{r};
	
	//! @test  Verify value was copied
	EXPECT_FALSE(r.empty());
	EXPECT_FALSE(s.empty());

	//! @test  Verify runtime type was copied
	EXPECT_EQ(VT_DISPATCH, r.kind());
	EXPECT_EQ(VT_DISPATCH, s.kind());
}

TEST(Variant_UT, MoveConstructor_TransfersInput)
{
	variant r{L"abc"}, s{std::move(r)};
	
	//! @test  Verify value was moved
	EXPECT_EQ(L"abc", (wstring)s);
	EXPECT_EQ(variant{}, r);
	
	//! @test  Verify runtime type was transferred
	EXPECT_EQ(VT_BSTR, s.kind());
	EXPECT_EQ(VT_EMPTY, r.kind());
}

TEST(Variant_UT, MoveConstructor_MaintainsRefCountWhenIUnknown)
{
	auto obj = make_mock_coclass<StrictMock<MockComObject>>();
	
	//! @post  Move-construction does not cause calls to @c AddRef() and @c Release()
	EXPECT_CALL(*obj, AddRef).Times(1);
	EXPECT_CALL(*obj, Release).Times(1);

	variant r{obj.get()}, s{std::move(r)};
	
	//! @test  Verify value was moved
	EXPECT_TRUE(r.empty());
	EXPECT_FALSE(s.empty());

	//! @test  Verify runtime type was moved
	EXPECT_EQ(VT_EMPTY, r.kind());
	EXPECT_EQ(VT_UNKNOWN, s.kind());
}

TEST(Variant_UT, MoveConstructor_MaintainsRefCountWhenIDispatch)
{
	auto obj = make_mock_coclass<StrictMock<MockDispatchObject>>();
	
	//! @post  Move-construction does not cause calls to @c AddRef() and @c Release()
	EXPECT_CALL(*obj, AddRef).Times(1);
	EXPECT_CALL(*obj, Release).Times(1);

	variant r{obj.get()}, s{std::move(r)};
	
	//! @test  Verify value was moved
	EXPECT_TRUE(r.empty());
	EXPECT_FALSE(s.empty());

	//! @test  Verify runtime type was moved
	EXPECT_EQ(VT_EMPTY, r.kind());
	EXPECT_EQ(VT_DISPATCH, s.kind());
}

TEST(Variant_UT, Constructor_BumpsRefCountWhenIUnknown)
{
	auto obj = make_mock_coclass<StrictMock<MockComObject>>();

	//! @post  Construction cause pair of calls to @c AddRef() and @c Release()
	EXPECT_CALL(*obj, AddRef).Times(1);
	EXPECT_CALL(*obj, Release).Times(1);

	variant const r{obj.get()};
	
	//! @test  Verify value is present
	EXPECT_FALSE(r.empty());

	//! @test  Verify runtime type
	EXPECT_EQ(VT_UNKNOWN, r.kind());
}

TEST(Variant_UT, Constructor_BumpsRefCountWhenIDispatch)
{
	auto obj = make_mock_coclass<StrictMock<MockDispatchObject>>();

	//! @post  Construction cause pair of calls to @c AddRef() and @c Release()
	EXPECT_CALL(*obj, AddRef).Times(1);
	EXPECT_CALL(*obj, Release).Times(1);

	variant const r{obj.get()};
	
	//! @test  Verify value is present
	EXPECT_FALSE(r.empty());

	//! @test  Verify runtime type
	EXPECT_EQ(VT_DISPATCH, r.kind());
}

TEST(Variant_UT, Constructor_BumpsRefCountWhenIUnknownSharedPtr)
{
	auto obj = make_mock_coclass<StrictMock<MockComObject>>();

	//! @post  Construction cause pair of calls to @c AddRef() and @c Release()
	EXPECT_CALL(*obj, AddRef).Times(1);
	EXPECT_CALL(*obj, Release).Times(1);
	
	variant const r{shared_ptr<::IUnknown>{obj.get()}};
	
	//! @test  Verify value is present
	EXPECT_FALSE(r.empty());

	//! @test  Verify runtime type
	EXPECT_EQ(VT_UNKNOWN, r.kind());
}

TEST(Variant_UT, Constructor_BumpsRefCountWhenIDispatchSharedPtr)
{
	auto obj = make_mock_coclass<StrictMock<MockDispatchObject>>();

	//! @post  Construction cause pair of calls to @c AddRef() and @c Release()
	EXPECT_CALL(*obj, AddRef).Times(1);
	EXPECT_CALL(*obj, Release).Times(1);

	variant const r{shared_ptr<::IDispatch>{obj.get()}};
	
	//! @test  Verify value is present
	EXPECT_FALSE(r.empty());

	//! @test  Verify runtime type
	EXPECT_EQ(VT_DISPATCH, r.kind());
}

TEST(Variant_UT, empty_TrueWhenEmpty) 
{
	//! @test  Verify @c empty() detects VT_EMPTY
	EXPECT_TRUE(variant{}.empty());
}

TEST(Variant_UT, empty_FalseWhenNull)
{
	//! @test  Verify @c empty() considers VT_NULL as non-empty
	EXPECT_FALSE(variant{nullptr}.empty());
}

TEST(Variant_UT, empty_FalseWhenNonEmpty) 
{
	//! @test  Verify @c empty() is @c false for everything but @c VT_EMPTY
	EXPECT_FALSE(variant{::VARIANT{VT_BOOL}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_I1}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_UI1}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_I2}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_UI2}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_I4}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_UI4}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_INT}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_UINT}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_I8}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_UI8}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_R4}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_R8}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_DECIMAL}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_BSTR}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_CY}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_DATE}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_ERROR}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_DISPATCH}}.empty());
	EXPECT_FALSE(variant{::VARIANT{VT_UNKNOWN}}.empty());
}

TEST(Variant_UT, kind_CorrectWhenEmpty) 
{
	//! @test  Verify @e empty variants are @c VT_EMPTY
	EXPECT_EQ(VT_EMPTY, variant{}.kind());
}

TEST(Variant_UT, kind_CorrectWhenNull) 
{
	//! @test  Verify @e null variants are @c VT_NULL
	EXPECT_EQ(VT_NULL, variant{nullptr}.kind());
}

TEST(Variant_UT, kind_CorrectWhenShort) 
{
	//! @test  Verify @c short variants are @c VT_I2
	EXPECT_EQ(VT_I2, variant{(short)42}.kind());
}

TEST(Variant_UT, kind_CorrectWhenUnsignedShort) 
{
	//! @test  Verify <tt>unsigned short</tt> variants are @c VT_UI2
	EXPECT_EQ(VT_UI2, variant{(unsigned short)42u}.kind());
}

TEST(Variant_UT, kind_CorrectWhenInt)
{
	//! @test  Verify @c int variants are @c VT_INT
	EXPECT_EQ(VT_INT, variant{42}.kind());
}

TEST(Variant_UT, kind_CorrectWhenUnsignedInt)
{
	//! @test  Verify @c unsigned variants are @c VT_UINT
	EXPECT_EQ(VT_UINT, variant{42u}.kind());
}

TEST(Variant_UT, kind_CorrectWhenLong)
{
	//! @test  Verify @c long variants are @c VT_I4
	EXPECT_EQ(VT_I4, variant{42l}.kind());
}

TEST(Variant_UT, kind_CorrectWhenUnsignedLong)
{
	//! @test  Verify <tt>unsigned long</tt> variants are @c VT_UI4
	EXPECT_EQ(VT_UI4, variant{42ul}.kind());
}

TEST(Variant_UT, kind_CorrectWhenLongLong)
{
	//! @test  Verify <tt>long long</tt> variants are @c VT_I8
	EXPECT_EQ(VT_I8, variant{42ll}.kind());
}

TEST(Variant_UT, kind_CorrectWhenUnsignedLongLong)
{
	//! @test  Verify <tt>unsigned long long</tt> variants are @c VT_UI8
	EXPECT_EQ(VT_UI8, variant{42ull}.kind());
}

TEST(Variant_UT, kind_CorrectWhenFloat) 
{
	//! @test  Verify @c float variants are @c VT_R4
	EXPECT_EQ(VT_R4, variant{42.0f}.kind());
}

TEST(Variant_UT, kind_CorrectWhenDouble) 
{
	//! @test  Verify @c double variants are @c VT_R8
	EXPECT_EQ(VT_R8, variant{42.0}.kind());
}

TEST(Variant_UT, kind_CorrectWhenString)
{
	//! @test  Verify @e string variants are @c VT_BSTR
	EXPECT_EQ(VT_BSTR, variant{"abc"}.kind());
}

TEST(Variant_UT, kind_CorrectWhenHResult) 
{
	//! @test  Verify @c win::HResult variants are @c VT_ERROR
	EXPECT_EQ(VT_ERROR, variant{win::HResult{E_FAIL}}.kind());
}

TEST(Variant_UT, kind_CorrectWhenIUnknown) 
{
	auto obj = make_mock_coclass<NiceMock<MockComObject>>();

	//! @test  Verify @c IUnknown variants are @c VT_UNKNOWN
	EXPECT_EQ(VT_UNKNOWN, variant{obj.get()}.kind());
}

TEST(Variant_UT, kind_CorrectWhenIDispatch) 
{
	auto obj = make_mock_coclass<NiceMock<MockDispatchObject>>();

	//! @test  Verify @c IDispatch variants are @c VT_DISPATCH
	EXPECT_EQ(VT_DISPATCH, variant{obj.get()}.kind());
}

TEST(Variant_UT, str_CorrectWhenEmpty) 
{
	//! @test  Verify @e empty variants return fixed string
	EXPECT_EQ("Empty", variant{}.str());
}

TEST(Variant_UT, str_CorrectWhenNull) 
{
	//! @test  Verify @e null variants return fixed string
	EXPECT_EQ("Null", variant{nullptr}.str());
}

TEST(Variant_UT, str_CorrectWhenBool) 
{
	//! @test  Verify @c bool variants return string-representation of their value
	EXPECT_EQ("True", variant{true}.str());

	//! @test  Verify @c bool variants return string-representation of their value
	EXPECT_EQ("False", variant{false}.str());
}

TEST(Variant_UT, str_CorrectWhenChar) 
{
	//! @test  Verify @c char variants return string-representation of their value
	EXPECT_EQ("42", variant{(char)42}.str());
}

TEST(Variant_UT, str_CorrectWhenByte) 
{
	//! @test  Verify <tt>unsigned char</tt> variants return string-representation of their value
	EXPECT_EQ("42", variant{(unsigned char)42}.str());
}

TEST(Variant_UT, str_CorrectWhenShort) 
{
	//! @test  Verify @c short variants return string-representation of their value
	EXPECT_EQ("42", variant{(short)42}.str());
}

TEST(Variant_UT, str_CorrectWhenUnsignedShort) 
{
	//! @test  Verify <tt>unsigned short</tt> variants return string-representation of their value
	EXPECT_EQ("42", variant{(unsigned short)42}.str());
}

TEST(Variant_UT, str_CorrectWhenInt) 
{
	//! @test  Verify @c int variants return string-representation of their value
	EXPECT_EQ("42", variant{42}.str());
}

TEST(Variant_UT, str_CorrectWhenUnsignedInt) 
{
	//! @test  Verify <tt>unsigned short</tt> variants return string-representation of their value
	EXPECT_EQ("42", variant{42u}.str());
}

TEST(Variant_UT, str_CorrectWhenLong)
{
	//! @test  Verify @c long variants return string-representation of their value
	EXPECT_EQ("42", variant{42l}.str());
}

TEST(Variant_UT, str_CorrectWhenUnsignedLong) 
{
	//! @test  Verify <tt>unsigned long</tt> variants return string-representation of their value
	EXPECT_EQ("42", variant{42ul}.str());
}

TEST(Variant_UT, str_CorrectWhenLongLong)
{
	//! @test  Verify <tt>long long</tt> variants return string-representation of their value
	EXPECT_EQ("42", variant{42ll}.str());
}

TEST(Variant_UT, str_CorrectWhenUnsignedLongLong) 
{
	//! @test  Verify <tt>unsigned long long</tt> variants return string-representation of their value
	EXPECT_EQ("42", variant{42ull}.str());
}

TEST(Variant_UT, str_CorrectWhenFloat)
{
	//! @test  Verify @c float variants return string-representation of their value
	EXPECT_EQ("42.1", variant{42.1f}.str());
}

TEST(Variant_UT, str_CorrectWhenDouble)
{
	//! @test  Verify @c double variants return string-representation of their value
	EXPECT_EQ("42.1", variant{42.1}.str());
}

TEST(Variant_UT, str_CorrectWhenDecimal)
{
	FAIL();
}

TEST(Variant_UT, str_CorrectWhenString)
{
	//! @test  Verify narrow-string variants return their value
	EXPECT_EQ("str", variant{"str"}.str());
	
	//! @test  Verify wide-string variants return their value
	EXPECT_EQ("str", variant{L"str"}.str());
}

TEST(Variant_UT, str_CorrectWhenDate)
{
	FAIL();
}

TEST(Variant_UT, str_CorrectWhenHResult)
{
	//! @test  Verify @c win::HResult variants return string-representation of their value
	EXPECT_EQ("S_OK", variant{win::HResult{S_OK}}.str());
}

TEST(Variant_UT, str_CorrectWhenCurrency)
{
	FAIL();
}

TEST(Variant_UT, str_CorrectWhenIUnknown)
{
	auto obj = make_mock_coclass<NiceMock<MockComObject>>();
	
	//! @test  Verify @c IUnknown variants return a fixed string
	EXPECT_EQ(L"IUnknown", variant{obj.get()}.wstr());
}

TEST(Variant_UT, str_CorrectWhenIDispatch)
{
	auto obj = make_mock_coclass<NiceMock<MockDispatchObject>>();
	
	//! @test  Verify @c IDispatch variants return a fixed string
	EXPECT_EQ(L"IDispatch", variant{obj.get()}.wstr());
}

TEST(Variant_UT, wstr_CorrectWideStringRepresentation) 
{	
	//! @test  Verify string variants return their value
	EXPECT_EQ(L"wstr", variant{L"wstr"}.wstr());

	//! @test  Verify non-string variants return their string representation
	EXPECT_EQ(L"42", variant{42}.wstr());
}

TEST(Variant_UT, valid_FalseForInvalidRuntimeType) 
{
	//! @test  Verify variants with unrecognised runtime type are @e invalid
	EXPECT_FALSE(variant::valid(static_cast<::VARENUM>(VT_VERSIONED_STREAM + 1)));
}

TEST(Variant_UT, valid_TrueForValidRuntimeType) 
{
	//! @test  Verify variants with recognised runtime type are @e valid
	EXPECT_TRUE(variant::valid(VT_EMPTY));
	EXPECT_TRUE(variant::valid(VT_NULL));
	EXPECT_TRUE(variant::valid(VT_BOOL));
	EXPECT_TRUE(variant::valid(VT_I1));
	EXPECT_TRUE(variant::valid(VT_UI1));
	EXPECT_TRUE(variant::valid(VT_I2));
	EXPECT_TRUE(variant::valid(VT_UI2));
	EXPECT_TRUE(variant::valid(VT_INT));
	EXPECT_TRUE(variant::valid(VT_UINT));
	EXPECT_TRUE(variant::valid(VT_I4));
	EXPECT_TRUE(variant::valid(VT_UI4));
	EXPECT_TRUE(variant::valid(VT_I8));
	EXPECT_TRUE(variant::valid(VT_UI8));
	EXPECT_TRUE(variant::valid(VT_R4));
	EXPECT_TRUE(variant::valid(VT_R8));
	EXPECT_TRUE(variant::valid(VT_DECIMAL));
	EXPECT_TRUE(variant::valid(VT_BSTR));
	EXPECT_TRUE(variant::valid(VT_CY));
	EXPECT_TRUE(variant::valid(VT_DATE));
	EXPECT_TRUE(variant::valid(VT_ERROR));
	EXPECT_TRUE(variant::valid(VT_UNKNOWN));
	EXPECT_TRUE(variant::valid(VT_DISPATCH));
}

TEST(Variant_UT, assignment_ValueChangedWhenBool)
{
	variant obj{true};
	obj = false;

	//! @test  Verify assignment changed value
	EXPECT_EQ(false, (bool)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotBool)
{
	variant obj{42};
	obj = true;

	//! @test  Verify assignment changed value
	EXPECT_EQ(true, (bool)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_BOOL, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenChar)
{
	variant obj{(char)42};
	obj = (char)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (char)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotChar)
{
	variant obj{42};
	obj = (char)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (char)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_I1, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenByte)
{
	variant obj{(unsigned char)42};
	obj = (unsigned char)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (unsigned char)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotByte)
{
	variant obj{42};
	obj = (unsigned char)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (unsigned char)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UI1, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenShort)
{
	variant obj{(short)42};
	obj = (short)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (short)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotShort)
{
	variant obj{42};
	obj = (short)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (short)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_I2, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenUnsignedShort)
{
	variant obj{(unsigned short)42};
	obj = (unsigned short)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (unsigned short)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotUnsignedShort)
{
	variant obj{42};
	obj = (unsigned short)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (unsigned short)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UI2, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenInt)
{
	variant obj{42};
	obj = 43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (int)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotInt)
{
	variant obj{true};
	obj = 43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43, (int)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_INT, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenUnsignedInt)
{
	variant obj{42u};
	obj = 43u;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43u, (unsigned)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotUnsignedInt)
{
	variant obj{false};
	obj = (unsigned)43;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43u, (unsigned)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UINT, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenLong)
{
	variant obj{42l};
	obj = 43l;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43l, (long)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotLong)
{
	variant obj{(short)22};
	obj = 43l;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43l, (long)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_I4, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenUnsignedLong)
{
	variant obj{42ul};
	obj = 43ul;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ul, (unsigned long)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotUnsignedLong)
{
	variant obj{"42"};
	obj = 43ul;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ul, (unsigned long)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UI4, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenLongLong)
{
	variant obj{42ll};
	obj = 43ll;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ll, (long long)obj);
}

#if (_WIN32_WINNT >= 0x0501)
TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotLongLong)
{
	variant obj{42.0f};
	obj = 43ll;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ll, (long long)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_I8, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenUnsignedLongLong)
{
	variant obj{42ull};
	obj = 43ull;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ull, (unsigned long long)obj);
}
#endif

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotUnsignedLongLong)
{
	variant obj{(short)42};
	obj = 43ull;

	//! @test  Verify assignment changed value
	EXPECT_EQ(43ull, (unsigned long long)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UI8, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenFloat)
{
	variant obj{42.2};
	obj = 43.3;

	//! @test  Verify assignment changed value
	EXPECT_FLOAT_EQ(43.3f, (float)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotFloat)
{
	variant obj{42};
	obj = 43.0f;

	//! @test  Verify assignment changed value
	EXPECT_FLOAT_EQ(43.0f, (float)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_R4, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenDouble)
{
	variant obj{42.0};
	obj = 43.0;

	//! @test  Verify assignment changed value
	EXPECT_DOUBLE_EQ(43.0, (double)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotDouble)
{
	variant obj{win::HResult{S_OK}};
	obj = 43.0;

	//! @test  Verify assignment changed value
	EXPECT_DOUBLE_EQ(43.0, obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_R8, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenString)
{
	variant obj{"abc"};
	obj = "ac";

	//! @test  Verify assignment changed value
	EXPECT_EQ(L"ac", (wstring)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotString)
{
	variant obj{42.0};
	obj = "43";

	//! @test  Verify assignment changed value
	EXPECT_EQ(L"43", (wstring)obj);
	
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
	variant obj{win::HResult{E_FAIL}};
	obj = win::HResult{S_OK};

	//! @test  Verify assignment changed value
	EXPECT_EQ(S_OK, (win::HResult)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotHResult)
{
	variant obj{(unsigned char)42};
	obj = win::HResult{E_OUTOFMEMORY};

	//! @test  Verify assignment changed value
	EXPECT_EQ(E_OUTOFMEMORY, (win::HResult)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_ERROR, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenIUnknown)
{
	auto v1 = make_mock_coclass<NiceMock<MockComObject>>();
	auto v2 = make_mock_coclass<NiceMock<MockComObject>>();

	variant obj{v1.get()};
	obj = v2.get();

	//! @test  Verify assignment changed value
	EXPECT_EQ(v2.get(), (shared_ptr<::IUnknown>)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotIUnknown)
{
	auto v1 = make_mock_coclass<NiceMock<MockComObject>>();
	
	variant obj{42.0f};
	obj = v1.get();

	//! @test  Verify assignment changed value
	EXPECT_EQ(v1.get(), (shared_ptr<::IUnknown>)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_UNKNOWN, obj.kind());
}

TEST(Variant_UT, assignment_ValueChangedWhenIDispatch)
{
	auto v1 = make_mock_coclass<NiceMock<MockDispatchObject>>();
	auto v2 = make_mock_coclass<NiceMock<MockDispatchObject>>();

	variant obj{v1.get()};
	obj = v2.get();

	//! @test  Verify assignment changed value
	EXPECT_EQ(v2.get(), (shared_ptr<::IDispatch>)obj);
}

TEST(Variant_UT, assignment_ValueAndTypeChangedWhenNotIDispatch)
{
	auto v1 = make_mock_coclass<NiceMock<MockDispatchObject>>();
	
	variant obj{42.0f};
	obj = v1.get();

	//! @test  Verify assignment changed value
	EXPECT_EQ(v1.get(), (shared_ptr<::IDispatch>)obj);
	
	//! @test  Verify assignment changed runtime type
	EXPECT_EQ(VT_DISPATCH, obj.kind());
}

TEST(Variant_UT, conversion_IntegerConversionsSucceedWhenValueCanBeRepresented) 
{
	//! @test  Verify integer conversions succeed
	EXPECT_EQ(true, (bool)variant{42});

	//! @test  Verify integer conversions succeed
	EXPECT_EQ(42, (char)variant{(unsigned char)42});

	//! @test  Verify integer conversions succeed
	EXPECT_EQ(42, (char)variant{(short)42});
	
	//! @test  Verify integer conversions succeed
	EXPECT_EQ(42, (short)variant{(int)42});

	//! @test  Verify integer conversions succeed
	EXPECT_EQ(42, (int)variant{(long)42});

	//! @test  Verify integer conversions succeed
	EXPECT_EQ(42, (long)variant{(long long)42});
}

TEST(Variant_UT, conversion_IntegerPromotionsSucceedWhenValueCanBeRepresented) 
{
	//! @test  Verify integer promotions succeed
	EXPECT_EQ(42, (short)variant{(char)42});

	//! @test  Verify integer promotions succeed
	EXPECT_EQ(42u, (unsigned short)variant{(char)42});
	
	//! @test  Verify integer promotions succeed
	EXPECT_EQ(42, (signed)variant{(short)42});

	//! @test  Verify integer promotions succeed
	EXPECT_EQ(42u, (unsigned)variant{(short)42});
	
	//! @test  Verify integer promotions succeed
	EXPECT_EQ(42, (long)variant{42});
	
	//! @test  Verify integer promotions succeed
	EXPECT_EQ(42u, (unsigned long)variant{42});
	
	//! @test  Verify integer promotions succeed
	EXPECT_EQ(42, (long long)variant{42l});
	
	//! @test  Verify integer promotions succeed
	EXPECT_EQ(42u, (unsigned long long)variant{42l});
}

TEST(Variant_UT, conversion_FloatingPointConversionsSucceedWhenValueCanBeRepresented) 
{
	//! @test  Verify floating-point conversions succeed
	EXPECT_FLOAT_EQ(42.1f, (float)variant{42.1});
}

TEST(Variant_UT, conversion_FloatingPointPromotionsSucceedWhenValueCanBeRepresented) 
{
	//! @test  Verify floating-point promotions succeed
	EXPECT_DOUBLE_EQ(42.1, (double)variant{42.1f});
}

TEST(Variant_UT, conversion_IntegerToStringConversionsSucceedsWhenValueCanBeRepresented) 
{
	//! @test  Verify integer-to-string conversions succeed
	EXPECT_EQ(L"42", (BinaryString)variant{42});
}

TEST(Variant_UT, conversion_StringToIntegerSucceedsWhenValueCanBeRepresented) 
{
	//! @test  Verify string-to-integer conversions succeed
	EXPECT_EQ(42, (int)variant{L"42"});
}

TEST(Variant_UT, conversion_FailsWhenValueCannotBeRepresented) 
{
	//! @test  Verify conversions fail when overflow would occur
	EXPECT_THROW((short)variant{INT_MAX}, range_error);
}

TEST(Variant_UT, conversion_FailsWhenConversionIsIllegal) 
{
	//! @test  Verify illegal type conversions fail
	EXPECT_THROW((shared_ptr<::IUnknown>)variant{42}, logic_error);
	
	//! @test  Verify illegal type conversions fail
	EXPECT_THROW((shared_ptr<::IDispatch>)variant{42}, logic_error);
}

TEST(Variant_UT, conversion_FailsWhenRuntimeTypeIsInvalid) 
{
	::VARIANT const Invalid{VT_VERSIONED_STREAM + 1};
	
	//! @test  Verify conversions fails when runtime type is invalid
	EXPECT_ANY_THROW((int)variant{Invalid});
}

TEST(Variant_UT, equality_WhenBothOperandsEmpty)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{}, variant{});

	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{nullptr}, variant{nullptr});
}

TEST(Variant_UT, equality_WhenBothOperandsBool)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{true}, variant{true});
	
	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{false}, variant{true});
}

TEST(Variant_UT, equality_WhenBothOperandsChar)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(char)1}, variant{(char)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(char)2}, variant{(char)1});
}

TEST(Variant_UT, equality_WhenBothOperandsByte)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(unsigned char)1}, variant{(unsigned char)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(unsigned char)2}, variant{(unsigned char)1});
}

TEST(Variant_UT, equality_WhenBothOperandsShort)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(short)1}, variant{(short)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(short)2}, variant{(short)1});
}

TEST(Variant_UT, equality_WhenBothOperandsUnsignedShort)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(unsigned short)1}, variant{(unsigned short)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(unsigned short)2}, variant{(unsigned short)1});
}

TEST(Variant_UT, equality_WhenBothOperandsInt)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(signed)1}, variant{(signed)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(signed)2}, variant{(signed)1});
}

TEST(Variant_UT, equality_WhenBothOperandsUnsignedInt)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(unsigned)1}, variant{(unsigned)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(unsigned)2}, variant{(unsigned)1});
}

TEST(Variant_UT, equality_WhenBothOperandsLong)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(long)1}, variant{(long)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(long)2}, variant{(long)1});
}

TEST(Variant_UT, equality_WhenBothOperandsUnsignedLong)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(unsigned long)1}, variant{(unsigned long)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(unsigned long)2}, variant{(unsigned long)1});
}

TEST(Variant_UT, equality_WhenBothOperandsLongLong)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(long long)1}, variant{(long long)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(long long)2}, variant{(long long)1});
}

TEST(Variant_UT, equality_WhenBothOperandsUnsignedLongLong)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{(unsigned long long)1}, variant{(unsigned long long)1});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{(unsigned long long)2}, variant{(unsigned long long)1});
}

TEST(Variant_UT, equality_WhenBothOperandsFloat)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_FLOAT_EQ(variant{1.0f}, variant{1.0f});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{2.0f}, variant{1.0f});
}

TEST(Variant_UT, equality_WhenBothOperandsDouble)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_DOUBLE_EQ(variant{1.0}, variant{1.0});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{2.0}, variant{1.0});
}

TEST(Variant_UT, equality_WhenBothOperandsDecimal)
{
	FAIL();
}

TEST(Variant_UT, equality_WhenBothOperandsString)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{L"abc"}, variant{L"abc"});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{L"abc"}, variant{L"ABC"});
}

TEST(Variant_UT, equality_WhenBothOperandsCurrency)
{
	FAIL();
}

TEST(Variant_UT, equality_WhenBothOperandsDate)
{
	FAIL();
}

TEST(Variant_UT, equality_WhenBothOperandsHResult)
{
	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{win::HResult{E_FAIL}}, variant{win::HResult{E_FAIL}});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{win::HResult{S_OK}}, variant{win::HResult{E_FAIL}});
}

TEST(Variant_UT, equality_WhenBothOperandsIUnknown)
{
	auto obj1 = make_mock_coclass<NiceMock<MockComObject>>();
	auto obj2 = make_mock_coclass<NiceMock<MockComObject>>();

	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{obj1.get()}, variant{obj1.get()});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{obj2.get()}, variant{obj1.get()});
}

TEST(Variant_UT, equality_WhenBothOperandsIDispatch)
{
	auto obj1 = make_mock_coclass<NiceMock<MockDispatchObject>>();
	auto obj2 = make_mock_coclass<NiceMock<MockDispatchObject>>();

	//! @test  Verify equality when runtime type are the same and values are equal
	EXPECT_EQ(variant{obj1.get()}, variant{obj1.get()});

	//! @test  Verify inequality when runtime type are the same and values are different
	EXPECT_NE(variant{obj2.get()}, variant{obj1.get()});
}

TEST(Variant_UT, equality_WhenOperandsExhibitReferenceEquality)
{
	variant v{42};
	
	//! @test  Verify equality when comparing against itself
	EXPECT_EQ(v, v);
}

TEST(Variant_UT, equality_WhenEitherOperandIsCorrupt)
{
	::VARIANT Invalid { VT_VERSIONED_STREAM+1 };

	//! @test  Verify corrupt variants cannot be compared
	EXPECT_ANY_THROW(variant{&Invalid} == Invalid);
	EXPECT_ANY_THROW(Invalid == variant{&Invalid});
}

TEST(Variant_UT, inequality_WhenOperandsDifferentType)
{
	//! @test  Verify inequality when runtime type are different
	EXPECT_NE(variant{}, variant{nullptr});
	EXPECT_NE(variant{}, variant{true});

	//! @test  Verify inequality when runtime type are different but values the same
	EXPECT_NE(variant{(char)42}, variant{(short)42});
	EXPECT_NE(variant{(short)42}, variant{(signed)42});
	EXPECT_NE(variant{(signed)42}, variant{(long)42});
	EXPECT_NE(variant{(long)42}, variant{(long long)42});
	EXPECT_NE(variant{(float)42.0f}, variant{(double)42.0});

	//! @test  Verify inequality when runtime type are different but values equivalent
	EXPECT_NE(variant{42}, variant{42.0f});
	EXPECT_NE(variant{false}, variant{0});
	EXPECT_NE(variant{true}, variant{1});
	EXPECT_NE(variant{"42"}, variant{42});
}

TEST(Variant_UT, roundtrip_BoolValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(true, (bool)variant{true});
}

TEST(Variant_UT, roundtrip_CharValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ((char)42, (char)variant{42});
}

TEST(Variant_UT, roundtrip_UnsignedCharValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ((unsigned char)42, (unsigned char)variant{42});
}

TEST(Variant_UT, roundtrip_ShortValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ((short)42, (short)variant{42});
}

TEST(Variant_UT, roundtrip_UnsignedShortValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ((unsigned short)42, (unsigned short)variant{42});
}

TEST(Variant_UT, roundtrip_IntValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(42, (int)variant{42});
}

TEST(Variant_UT, roundtrip_UnsignedIntValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(42u, (unsigned)variant{42u});
}

TEST(Variant_UT, roundtrip_LongValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(42l, (int)variant{42l});
}

TEST(Variant_UT, roundtrip_UnsignedLongValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(42ul, (unsigned)variant{42ul});
}

#if (_WIN32_WINNT >= 0x0501)
	TEST(Variant_UT, roundtrip_LongLongValueIsUnchanged) 
	{
		//! @test  Verify value is unmodified
		EXPECT_EQ(42ll, (int)variant{42ll});
	}

	TEST(Variant_UT, roundtrip_UnsignedLongLongValueIsUnchanged) 
	{
		//! @test  Verify value is unmodified
		EXPECT_EQ(42ull, (unsigned)variant{42ull});
	}
#endif

TEST(Variant_UT, roundtrip_FloatValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_FLOAT_EQ(42.2f, (float)variant{42.2});
}

TEST(Variant_UT, roundtrip_DoubleValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_DOUBLE_EQ(42.2, (double)variant{42.2});
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
	EXPECT_EQ(L"abc", (wstring)variant{"abc"});
}

TEST(Variant_UT, roundtrip_WideStringValueIsUnchanged) 
{
	//! @test  Verify value is unmodified
	EXPECT_EQ(L"wabc", (wstring)variant{L"wabc"});
}

TEST(Variant_UT, roundtrip_IUnknownValueIsUnchanged) 
{
	auto v1 = make_mock_coclass<NiceMock<MockComObject>>();
	
	//! @test  Verify value is unmodified
	EXPECT_EQ(v1.get(), (shared_ptr<::IUnknown>)variant{v1.get()});
	
	//! @test  Verify value is unmodified
	EXPECT_EQ(v1.get(), (shared_ptr<::IUnknown>)variant{shared_ptr<::IUnknown>{v1.get()}});
}

TEST(Variant_UT, roundtrip_IDispatchValueIsUnchanged) 
{
	auto v1 = make_mock_coclass<NiceMock<MockDispatchObject>>();
	
	//! @test  Verify value is unmodified
	EXPECT_EQ(v1.get(), (shared_ptr<::IDispatch>)variant{shared_ptr<::IDispatch>{v1.get()}});
}
} // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#endif	// DISABLE_VARIANT_UNIT_TESTS