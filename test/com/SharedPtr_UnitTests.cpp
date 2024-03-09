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
//#define DISABLE_SHARED_PTR_UNIT_TESTS
#ifndef DISABLE_SHARED_PTR_UNIT_TESTS
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include <gtest/gtest.h>
#include "com/SharedPtr.h"
#include "com/Annotations.h"
#include "core/LiteralString.h"
#include "com/MockDispatch.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
using namespace core;
using namespace com::literals;
using namespace ::testing;
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
[emitidl(restricted)];

[object, uuid("FE2C8ED8-98DA-4BF7-BD5E-0AA1D38FB5E6")]
__interface IExample : IUnknown
{
	HRESULT 
	Method(long idx, com::in_t<wchar_t const> str, com::retval_t<long> out);
};

[uuid("42C386F4-95A0-43A7-B24C-7288D31E98C2")]
class Example : public com::implements<IExample,::IUnknown>
{
public:
	struct library_type {
		auto constexpr 
		static library_name = core::LiteralString{"ExampleLib"};

		auto constexpr 
		static library_guid = "D0DCF441-AA4C-4EAC-ACC1-32EDD2ED16F6"_guid;
	};
	
public:
	Example() = default;

public:
	HRESULT 
	COMAPI Method(long idx, com::in_t<wchar_t const> str, com::retval_t<long> out) override
	{
		return S_OK;
	}
};
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
TEST(SharedPtr_UT, DefaultConstructor_ConstructsEmpty)
{
	//! @test  Verify default constructed instance is initially @e empty
	EXPECT_TRUE(shared_ptr<::IUnknown>{}.empty());
}

TEST(SharedPtr_UT, Constructor_CallsAddRef)
{
	auto obj = make_mock_coclass<StrictMock<MockUnknown>>();
	
	//! @post  Object receives call to @c AddRef()
	EXPECT_CALL(*obj, AddRef).Times(1);

	//! @test  Verify constructor increases ref-count
	shared_ptr<::IUnknown>{obj.get()}.detach();
}

TEST(SharedPtr_UT, Constructor_ConstructsEmptyIfNull)
{
	//! @test  Verify constructed instance is @e empty
	EXPECT_TRUE(shared_ptr<::IUnknown>{(::IUnknown*)nullptr}.empty());
}

TEST(SharedPtr_UT, AdoptConstructor_DoesntCallAddRef)
{
	//! @post  Object doesn't receive a call to @c AddRef()
	auto obj = make_mock_coclass<StrictMock<MockUnknown>>();

	//! @test  Verify adopt constructor doesn't affect ref-count
	shared_ptr<::IUnknown>{adopt, obj.get()}.detach();
}

TEST(SharedPtr_UT, CastConstructor_CallsQueryInterface)
{
	//! @post  Object returned by @c QueryInterface() doesn't receive a spurious call to @c AddRef()
	std::shared_ptr<MockDispatch> dispatch = make_mock_coclass<StrictMock<MockDispatch>>();

	//! @post  Object being cast receives call to @c QueryInterface()
	std::shared_ptr<MockUnknown> unknown = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*unknown, QueryInterface).Times(
		1
	).WillOnce(
		DoAll(
			SetArgPointee<1>(static_cast<void*>(dispatch.get())), 
			Return(S_OK)
		)
	);
	
	//! @test  Verify casting constructor maintains correct ref-count
	shared_ptr<::IUnknown> src{adopt, unknown.get()};
	shared_ptr<::IDispatch> dest{src};
	src.detach();
	dest.detach();
}

TEST(SharedPtr_UT, CopyConstructor_CallsAddRef)
{
	auto obj = make_mock_coclass<StrictMock<MockUnknown>>();
	
	//! @post  Object receives call to @c AddRef()
	EXPECT_CALL(*obj, AddRef).Times(1);

	//! @test  Verify copy-constructor increases ref-count
	shared_ptr<::IUnknown> src{adopt, obj.get()};
	shared_ptr<::IUnknown> copy{src};
	src.detach();
	copy.detach();
}

TEST(SharedPtr_UT, MoveConstructor_DoesntCallAddRef)
{
	//! @post  Object doesn't receives call to @c AddRef()
	auto obj = make_mock_coclass<StrictMock<MockUnknown>>();
	
	//! @test  Verify move-constructor increases ref-count
	shared_ptr<::IUnknown> src{adopt, obj.get()};
	shared_ptr<::IUnknown> dest{std::move(src)};
	dest.detach();

	//! @test  Verify source object is empty
	EXPECT_TRUE(src.empty());
}

TEST(SharedPtr_UT, CopyAssignment_CallsAddRefAndRelease)
{
	//! @post  Old object receives call to @c Release()
	auto oldValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*oldValue, Release).Times(1);

	//! @post  New object receives call to @c AddRef()
	auto newValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*newValue, AddRef).Times(1);

	//! @test  Verify copy-assignment decreases ref-count of prior value and increases ref-count of new value
	shared_ptr<::IUnknown> lhs{adopt, oldValue.get()};
	shared_ptr<::IUnknown> rhs{adopt, newValue.get()};
	lhs = rhs;

	//! @test  Verify operator didn't empty the source
	EXPECT_FALSE(rhs.empty());

	lhs.detach();
	rhs.detach();
}

TEST(SharedPtr_UT, CopyAssignment_CallsAddRefAndReleaseOnRawPointer)
{
	//! @post  Old object receives call to @c Release()
	auto oldValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*oldValue, Release).Times(1);

	//! @post  New object receives call to @c AddRef()
	auto newValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*newValue, AddRef).Times(1);

	//! @test  Verify copy-assignment decreases ref-count of prior value and increases ref-count of new value
	shared_ptr<::IUnknown> lhs{adopt, oldValue.get()};
	lhs = newValue.get();
	lhs.detach();
}

TEST(SharedPtr_UT, MoveAssignment_CallsReleaseOnly)
{
	//! @post  Old object receives call to @c Release()
	auto oldValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*oldValue, Release).Times(1);

	//! @post  New object receives no call to @c AddRef()
	auto newValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*newValue, AddRef).Times(0);

	//! @test  Verify move-assignment decreases ref-count of prior value but maintains ref-count of new value
	shared_ptr<::IUnknown> lhs{adopt, oldValue.get()};
	shared_ptr<::IUnknown> rhs{adopt, newValue.get()};
	lhs = std::move(rhs);
	lhs.detach();

	//! @test  Verify move-assignment leaves old object empty
	EXPECT_TRUE(rhs.empty());
}

TEST(SharedPtr_UT, Destructor_CallsRelease)
{
	auto obj = make_mock_coclass<StrictMock<MockUnknown>>();
	
	//! @post  Object receives call to @c Release()
	EXPECT_CALL(*obj, Release).Times(1);

	//! @test  Verify destructor decreases ref-count
	shared_ptr<::IUnknown>{adopt, obj.get()};
}

TEST(SharedPtr_UT, Detach_LeavesObjectEmpty)
{
	//! @post  @c Release() is not called upon destruction
	auto obj = make_mock_coclass<StrictMock<MockUnknown>>();

	//! @test  Verify object is @e empty following @c detach()
	shared_ptr<::IUnknown> ptr{adopt, obj.get()};
	ptr.detach();
	EXPECT_TRUE(ptr.empty());
}

TEST(SharedPtr_UT, Detach_ReturnsObjectProvidedAtConstruction)
{
	//! @post  @c Release() is not called upon destruction
	auto obj = make_mock_coclass<StrictMock<MockUnknown>>();
	
	//! @test  Verify correct object is returned
	shared_ptr<::IUnknown> ptr{adopt, obj.get()};
	EXPECT_EQ(obj.get(), ptr.detach());
}

TEST(SharedPtr_UT, Empty_ReturnsTrueWhenNull)
{
	//! @test  Verify returns @c true when pointer contains @c nullptr
	EXPECT_TRUE(shared_ptr<::IUnknown>{}.empty());
	EXPECT_TRUE(shared_ptr<::IUnknown>{(::IUnknown*)nullptr}.empty());
}

TEST(SharedPtr_UT, Empty_ReturnsTrueWhenNonNull)
{
	auto obj = make_mock_coclass<NiceMock<MockUnknown>>();

	//! @test  Verify returns @c false when pointer contains an object
	EXPECT_FALSE(shared_ptr<::IUnknown>{obj.get()}.empty());
}

TEST(SharedPtr_UT, equality_BetweenEmpty)
{
	//! @test  Verify empty instances compare equal
	EXPECT_EQ(shared_ptr<::IUnknown>{}, shared_ptr<::IUnknown>{});
	EXPECT_EQ(shared_ptr<::IUnknown>{(::IUnknown*)nullptr}, shared_ptr<::IUnknown>{});
	EXPECT_EQ(shared_ptr<::IUnknown>{}, shared_ptr<::IUnknown>{(::IUnknown*)nullptr});
}

TEST(SharedPtr_UT, equality_BetweenSameObject)
{
	auto obj1 = make_mock_coclass<NiceMock<MockUnknown>>();
	auto obj2 = make_mock_coclass<NiceMock<MockUnknown>>();

	//! @test  Verify equality between pointers holding same object
	EXPECT_EQ(shared_ptr<::IUnknown>{obj1.get()}, shared_ptr<::IUnknown>{obj1.get()});

	//! @test  Verify inequality between distinct objects
	EXPECT_NE(shared_ptr<::IUnknown>{obj2.get()}, shared_ptr<::IUnknown>{obj1.get()});
}

TEST(SharedPtr_UT, OperatorBool_ReturnsFalseWhenEmpty)
{
	//! @test  Verify returns @c false when pointer contains @c nullptr
	EXPECT_FALSE((bool)shared_ptr<::IUnknown>{});
	EXPECT_FALSE((bool)shared_ptr<::IUnknown>{(::IUnknown*)nullptr});
}

TEST(SharedPtr_UT, OperatorBool_ReturnsTrueWhenNonNull)
{
	auto obj = make_mock_coclass<NiceMock<MockUnknown>>();

	//! @test  Verify returns @c true when pointer contains an object
	EXPECT_TRUE((bool)shared_ptr<::IUnknown>{obj.get()});
}

TEST(SharedPtr_UT, OperatorIndirection_ReturnsObjectProvidedAtConstruction)
{
	auto obj = make_mock_coclass<NiceMock<MockUnknown>>();

	//! @test  Verify operator returns expected value
	EXPECT_EQ(obj.get(), shared_ptr<::IUnknown>{obj.get()}.operator->());
}

TEST(SharedPtr_UT, OperatorInterface_ReturnsObjectProvidedAtConstruction)
{
	auto obj = make_mock_coclass<NiceMock<MockUnknown>>();

	//! @test  Verify operator returns expected value
	EXPECT_EQ(obj.get(), (::IUnknown*)shared_ptr<::IUnknown>{obj.get()});
}

TEST(SharedPtr_UT, Reset_CallsRelease)
{
	auto obj = make_mock_coclass<StrictMock<MockUnknown>>();
	
	//! @post  Object receives call to @c Release()
	EXPECT_CALL(*obj, Release).Times(1);

	//! @test  Verify reset decreases ref-count
	shared_ptr<::IUnknown> ptr{adopt, obj.get()};
	ptr.reset();
	ptr.detach();
}

TEST(SharedPtr_UT, Reset_CallsAddRefAndReleaseOnRawPointer)
{
	//! @post  Old object receives call to @c Release()
	auto oldValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*oldValue, Release).Times(1);

	//! @post  New object receives call to @c AddRef()
	auto newValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*newValue, AddRef).Times(1);

	//! @test  Verify @c reset() decreases ref-count of prior value and increases ref-count of new value
	shared_ptr<::IUnknown> obj{adopt, oldValue.get()};
	obj.reset(newValue.get());
	obj.detach();
}

TEST(SharedPtr_UT, ResetAdopt_CallsReleaseOnly)
{
	//! @post  Old object receives call to @c Release()
	auto oldValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*oldValue, Release).Times(1);

	//! @post  New object receives no call to @c AddRef()
	auto newValue = make_mock_coclass<StrictMock<MockUnknown>>();
	EXPECT_CALL(*newValue, AddRef).Times(0);

	//! @test  Verify @c reset() only decreases ref-count of prior value
	shared_ptr<::IUnknown> obj{adopt, oldValue.get()};
	obj.reset(adopt, newValue.get());
	obj.detach();
}

TEST(SharedPtr_UT, MakeShared_ConstructsByTypeName)
{
	//! @test  Verify @c com::make_shared() succeeds when passed a complete C++ class type which implements a COM interface
	EXPECT_NE(nullptr, (make_shared<IExample,Example>()));
}

TEST(SharedPtr_UT, MakeShared_ConstructsByCLSID)
{
	GTEST_FATAL_FAILURE_("Requires a system-test with a registered COM object");
}
} // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#endif	// DISABLE_SHARED_PTR_UNIT_TESTS