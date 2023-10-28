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
#include "../../src/library/ComExport.h"
#include "com/Concepts.h"
#include "com/traits/distinct_interfaces.h"
#include "win/HResult.h"
#include "core/FunctionLogging.h"
#include "com/GlobalRefCount.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	//! @brief	Realizes an arbitrary set COM interfaces
	template <meta::ForwardSequence ForwardSequence>
	class MultipleRealization; 

	template <template<typename...> typename Sequence, meta::ComInterface... Interfaces>
	class MultipleRealization<Sequence<Interfaces...>> : public Interfaces...
	{
		satisfies(MultipleRealization,
			IsInterface,
			NotCopyable,
			NotEqualityComparable,
			NotSortable
		);
	};
	

	//! @brief	Implements @c ::IUnknown for a set of COM interfaces
	//! @tparam	Interfaces	Set of _all_ COM interfaces to be realized including ancestral interfaces (eg. @c ::IUnknown)
	template <meta::ComInterface... Interfaces>
	class implements : public MultipleRealization<distinct_interfaces_t<Interfaces...>>,
	                   protected GlobalRefCount
	{	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using type = implements<Interfaces...>;
		using base = MultipleRealization<distinct_interfaces_t<Interfaces...>>;

	public:
		using interfaces = mpl::vector<Interfaces...>;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		std::atomic_long refCount = 1;

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		implements() noexcept {
			GlobalRefCount::increment();
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(implements,
			IsPolymorphic,
			NotCopyable,
			NotEqualityComparable,
			NotSortable
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		::HRESULT
		COMAPI QueryInterface(::IID const& iid, void** ppv) override
		{
			win::HResult hr = S_OK;
			logFunctionArgs(iid,ppv).withRetVals(hr,*ppv);

			if (!ppv) 
				return hr = E_INVALIDARG;

			return hr = this->QueryInterfaceImpl<Interfaces...>(iid,ppv);
		}

		::ULONG
		COMAPI AddRef() override
		{
			logFunctionArgs().withRetVals(this->refCount, GlobalRefCount::SumInstances);

			return this->refCount++;
		}
	
		::ULONG
		COMAPI Release() override
		{
			logFunctionArgs().withRetVals(this->refCount, GlobalRefCount::SumInstances);

			if (this->refCount <= 0)
				clog << Warning{"Coclass has an invalid reference count of {}", this->refCount.load()};

			if (--this->refCount <= 0)
			{
				delete this;
				GlobalRefCount::decrement();
				return 0;
			}

			return this->refCount;
		}

	private:
		template <meta::ComInterface Interface, meta::ComInterface... Remainder>
		::HRESULT
		COMAPI QueryInterfaceImpl(::IID const& iid, void** ppv) 
		{
			if (__uuidof(Interface) == iid) 
			{
				// Since coclasses commonly realize multiple interfaces, each of which inherits from IUnknown,
				// ambiguous conversions to IUnknown commonly surface (ie. the diamond problem). COM has an
				// identity requirement stating that, whatever conversion we choose, it must be consistent; so
				// we're casting via the _first_ interface in the list.
				if constexpr (!std::is_convertible_v<type const*, Interface const*>)
					*ppv = static_cast<Interface*>(
						static_cast<typename mpl::front<interfaces>::type*>(this)
					);
					// FIXME: Only works for common case, we should select the path dynamically by searching the
					//        type-list for the first common base eg. std::common_with
				else
					*ppv = static_cast<Interface*>(this);
				this->AddRef();
				return S_OK;
			}

			if constexpr (nstd::AtLeastOneType<Remainder...>)
				return this->QueryInterfaceImpl<Remainder...>(iid, ppv);
			else
				return E_NOINTERFACE;
		}
	};
}  // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{
	using TestVector = mpl::vector<IClassFactory,IUnknown,ITypeInfo>;
	using TestVector_WithoutIUnknown = typename mpl::remove_if<TestVector, detail::is_strict_base_of<IClassFactory>>::type;
	static_assert(mpl::size<TestVector_WithoutIUnknown>::value == 2);
	static_assert(mpl::contains<TestVector_WithoutIUnknown,IClassFactory>::value);
	static_assert(!mpl::contains<TestVector_WithoutIUnknown,IUnknown>::value);
	static_assert(mpl::contains<TestVector_WithoutIUnknown,ITypeInfo>::value);

	using TestVector2 = mpl::vector<IClassFactory2,IClassFactory,IDispatch>;
	using TestVector2_WithoutIClassFactory = typename mpl::remove_if<TestVector2, detail::is_strict_base_of<IClassFactory2>>::type;
	static_assert(mpl::size<TestVector2_WithoutIClassFactory>::value == 2);
	static_assert(mpl::contains<TestVector2_WithoutIClassFactory,IClassFactory2>::value);
	static_assert(!mpl::contains<TestVector2_WithoutIClassFactory,IClassFactory>::value);
	static_assert(mpl::contains<TestVector2_WithoutIClassFactory,IDispatch>::value);
	
	using TestVector3 = distinct_interfaces_t<IClassFactory,IUnknown>;
	static_assert(mpl::size<TestVector3>::value == 1);
	static_assert(mpl::contains<TestVector3,IClassFactory>::value);
	static_assert(!mpl::contains<TestVector3,IUnknown>::value);

	using TestVector4 = distinct_interfaces_t<IClassFactory,IDispatch,IUnknown>;
	static_assert(mpl::size<TestVector4>::value == 2);
	static_assert(mpl::contains<TestVector4,IClassFactory>::value);
	static_assert(mpl::contains<TestVector4,IDispatch>::value);
	static_assert(!mpl::contains<TestVector4,IUnknown>::value);
}  // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o