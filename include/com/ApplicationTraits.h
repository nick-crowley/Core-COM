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
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.COM.h"
#include "core/LiteralString.h"
#include "com/Guid.h"
#include "com/Version.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::detail
{
	//! @brief	@c Application::application_guid if present, then @c __uuidof(Application) if valid; otherwise @c com::Guid{}
	template <typename Application, typename = void>
	Guid constexpr
	application_guid_v;

	template <typename Application> 
		requires 
			requires { Application::application_guid; }
	Guid constexpr
	application_guid_v<Application,void> = Application::application_guid;
	
	template <meta::HasGuid Application> 
		requires 
			(!requires { Application::application_guid; })
	Guid constexpr
	application_guid_v<Application,void> {__uuidof(Application)};
}

namespace core::meta::detail
{	
	//! @brief	Well-formed declaration containing mandatory minimum properties for a COM application
	template <typename Application>
	concept CoreApplicationDeclaration = 
	  com::detail::application_guid_v<Application> != com::Guid{};
}

namespace core::com
{
	//! @brief	Compile-time metadata for a COM application
	template <meta::detail::CoreApplicationDeclaration Application>
	metafunc application_traits
	{
		Guid constexpr
		static application_guid = detail::application_guid_v<Application>; 
	};
}

namespace core::meta
{
	//! @brief	Well-formed COM application possessesing valid traits (either deduced or explicitly specialized)
	template <typename T>
	concept CoreApplication = 
	  com::application_traits<T>::application_guid != com::Guid{};
}

namespace core::com
{
	//! @brief	GUID for well-formed COM application @p Application
	template <meta::CoreApplication Application>
	Guid constexpr
	application_guid_v = application_traits<Application>::application_guid;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{
	//! @test  Verify @c com::detail::application_guid_v is empty string when GUID not present
	struct ApplicationWithoutGuid{};
	static_assert(detail::application_guid_v<ApplicationWithoutGuid> == Guid{});

	//! @test  Verify @c com::detail::application_guid_v is detected from metadata
	using namespace guid_literals;
	[uuid("1B43E3B9-03CB-4724-8082-20BD8086DB05")]
	struct ApplicationWithAttributeGuid{};
	static_assert(detail::application_guid_v<ApplicationWithAttributeGuid> == "1B43E3B9-03CB-4724-8082-20BD8086DB05"_guid);
	
	//! @test  Verify @c com::detail::application_guid_v is detected from static member variable
	struct ApplicationWithMemberGuid{
		Guid constexpr
		static application_guid = Guid{"1B43E3B9-03CB-4724-8082-20BD8086DB05"};
	};
	static_assert(detail::application_guid_v<ApplicationWithMemberGuid> == "1B43E3B9-03CB-4724-8082-20BD8086DB05"_guid);
	
	//! @test  Verify @c com::detail::application_guid_v is the same whether detected from either source
	static_assert(detail::application_guid_v<ApplicationWithAttributeGuid> == detail::application_guid_v<ApplicationWithMemberGuid>);

	//! @test Verify application GUID is mandatory to model @c meta::detail::CoreApplicationDeclaration
	struct ApplicationWithEmptyGuid{
		Guid constexpr
		static application_guid;
	};
	static_assert(meta::detail::CoreApplicationDeclaration<ApplicationWithAttributeGuid>);
	static_assert(meta::detail::CoreApplicationDeclaration<ApplicationWithMemberGuid>);
	static_assert(!meta::detail::CoreApplicationDeclaration<ApplicationWithoutGuid>);
	static_assert(!meta::detail::CoreApplicationDeclaration<ApplicationWithEmptyGuid>);


	//! @test Verify application GUID is mandatory to model@c meta::CoreApplication
	[uuid("2DBFB846-5F59-4039-8E60-80870F63BC94")]
	struct ValidCoApplication{};
	static_assert(meta::CoreApplication<ValidCoApplication>);
	static_assert(!meta::CoreApplication<ApplicationWithoutGuid>);
	static_assert(!meta::CoreApplication<ApplicationWithEmptyGuid>);


	//! @test Verify trait accessors produce same values as their implementations
	static_assert(application_guid_v<ValidCoApplication> == detail::application_guid_v<ValidCoApplication>);
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o