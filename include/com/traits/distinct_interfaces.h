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
#ifndef CoreCom_h_included
#	error Including this header directly may cause a circular dependency; include <core.COM.h> directly
#endif
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "com/Concepts.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	// clang-format off
	namespace detail
	{
		//! @brief	Query whether @c Derived is a public and unambiguous descendant of @c Base (but is not @c Base)
		template <meta::ComInterface Derived>
		metafunc is_strict_base_of {
			template <meta::ComInterface Base>
			metafunc apply : std::conjunction<
				std::is_convertible<Derived const*,Base const*>,
				std::negation<std::is_same<Derived,Base>>
			>
			{};
		};
	
		//! @brief  Applies @c detail::is_strict_base_of to each element of @p ForwardSequence
		template <meta::ForwardSequence ForwardSequence>
		metafunc distinct_interfaces_impl : std::type_identity_t<ForwardSequence>{};

		template <template<typename...> typename ForwardSequence, meta::ComInterface I>
		metafunc distinct_interfaces_impl<ForwardSequence<I>> : std::type_identity_t<ForwardSequence<I>>{};

		template <template<typename...> typename ForwardSequence, meta::ComInterface I, typename ...R>
		metafunc distinct_interfaces_impl<ForwardSequence<I,R...>> : std::type_identity_t<
			typename mpl::remove_if<ForwardSequence<I,R...>,is_strict_base_of<I>>::type
		>{};
	}
	
	//! @brief	Sequence of distinct COM interfaces (ie. with ancestral interfaces removed)
	//! 
	//! @tparam	Interfaces...	Set of COM interfaces, possibly including common base classes
	template <meta::ComInterface... I>
	metafunc distinct_interfaces : detail::distinct_interfaces_impl<mpl::vector<I...>>
	{};

	// Type-accessor
	template <meta::ComInterface... Interfaces>
	using distinct_interfaces_t = typename distinct_interfaces<Interfaces...>::type;

	// clang-format on
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{
	//! @test  Verify @c com::distinct_interfaces_t<T...> removes child when passed two types in a parent-child relationship
	using JustIClassFactory = distinct_interfaces_t<IClassFactory,IUnknown>;
	static_assert(mpl::size<JustIClassFactory>::value == 1);
	static_assert(mpl::contains<JustIClassFactory,IClassFactory>::value);
	static_assert(!mpl::contains<JustIClassFactory,IUnknown>::value);
	
	//! @test  Verify @c com::distinct_interfaces_t<T...> removes common roots from the inheritance hierarchy
	using IClassFactoryAndIDispatch = distinct_interfaces_t<IClassFactory,IDispatch,IUnknown>;
	static_assert(mpl::size<IClassFactoryAndIDispatch>::value == 2);
	static_assert(mpl::contains<IClassFactoryAndIDispatch,IClassFactory>::value);
	static_assert(mpl::contains<IClassFactoryAndIDispatch,IDispatch>::value);
	static_assert(!mpl::contains<IClassFactoryAndIDispatch,IUnknown>::value);
}  // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o