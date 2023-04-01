#pragma once
#include "library/core.COM.h"
#include "com/HResult.h"
#include "core/FunctionLogging.h"

namespace core::com
{
	namespace detail
	{
		//! @brief	Realizes an arbitrary type-list of COM interfaces
		template <meta::ForwardSequence ForwardSequence>
		class MultipleRealization; 

		template <template<typename...> typename ForwardSequence, meta::Interface... Interfaces>
		class MultipleRealization<ForwardSequence<Interfaces...>> : public Interfaces...
		{
			satisfies(MultipleRealization,
				protected: IsDefaultConstructible noexcept,
				public: IsPolymorphic,
				NotCopyable,
				NotEqualityComparable,
				NotSortable
			);
		};
	
		//! @brief	Query whether @c Derived is a public and unambiguous descendant of @c Base (but is not @c Base)
		template <meta::Interface Derived>
		struct is_strict_base_of {
			template <meta::Interface Base>
			struct apply : std::conjunction<
				std::is_convertible<Derived const*,Base const*>,
				std::negation<std::is_same<Derived,Base>>
			>
			{};
		};
	
		template <meta::ForwardSequence ForwardSequence>
		struct distinct_interfaces_impl : std::type_identity_t<ForwardSequence>{};

		template <template<typename...> typename ForwardSequence, meta::Interface I>
		struct distinct_interfaces_impl<ForwardSequence<I>> : std::type_identity_t<ForwardSequence<I>>{};

		template <template<typename...> typename ForwardSequence, meta::Interface I, typename ...R>
		struct distinct_interfaces_impl<ForwardSequence<I,R...>> : std::type_identity_t<
			typename mpl::remove_if<ForwardSequence<I,R...>,is_strict_base_of<I>>::type
		>{};

		template <meta::Interface... I>
		struct distinct_interfaces : distinct_interfaces_impl<mpl::vector<I...>>
		{};

		//! @brief	Type-list of distinct COM interfaces (ie. with ancestral interfaces removed)
		//! @tparam	Interfaces...	Set of COM interfaces, possibly including common base classes
		template <meta::Interface... Interfaces>
		using distinct_interfaces_t = typename distinct_interfaces<Interfaces...>::type;
	}

	std::atomic_long 
	extern numInstances;

	//! @brief	Implements @c ::IUnknown for a set of COM interfaces
	//! @tparam	Interfaces	Set of _all_ COM interfaces to be realized including ancestral interfaces (eg. @c ::IUnknown)
	template <meta::Interface... Interfaces>
	class implements : public detail::MultipleRealization<detail::distinct_interfaces_t<Interfaces...>>
	{	
		using type = implements<Interfaces...>;

	public:
		using interfaces = mpl::vector<Interfaces...>;

	private:
		std::atomic_long refCount = 0;

	public:
		satisfies(implements,
			IsDefaultConstructible noexcept,
			IsPolymorphic,
			NotCopyable,
			NotEqualityComparable,
			NotSortable
		);

	public:
		::HRESULT
		__stdcall QueryInterface(::IID const& iid, void** ppv) override
		{
			HResult hr = S_OK;
			logFunctionArgs(iid,ppv).withRetVals(hr,*ppv);

			if (!ppv) 
				return hr = E_INVALIDARG;

			return hr = this->QueryInterfaceImpl<Interfaces...>(iid,ppv);
		}

		::ULONG
		__stdcall AddRef() override
		{
			logFunctionArgs().withRetVals(std::cref(this->refCount), std::cref(com::numInstances));

			if (this->refCount++ == 0) 
				++com::numInstances;

			return this->refCount;
		}
	
		::ULONG
		__stdcall Release() override
		{
			logFunctionArgs().withRetVals(std::cref(this->refCount), std::cref(com::numInstances));

			if (this->refCount <= 0)
				clog << Warning{"Coclass has an invalid reference count of {}", this->refCount.load()};

			if (--this->refCount <= 0)
			{
				delete this;
				--com::numInstances;
				return 0;
			}

			return this->refCount;
		}

	private:
		template <meta::Interface Interface, meta::Interface... Remainder>
		::HRESULT
		__stdcall QueryInterfaceImpl(::IID const& iid, void** ppv) 
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

			if constexpr (sizeof...(Remainder))
				return this->QueryInterfaceImpl<Remainder...>(iid, ppv);
			else
				return E_NOINTERFACE;
		}
	};
	
	namespace detail::testing
	{
		using TestVector = mpl::vector<IClassFactory,IUnknown,ITypeInfo>;
		using TestVector_WithoutIUnknown = typename mpl::remove_if<TestVector, is_strict_base_of<IClassFactory>>::type;
		static_assert(mpl::size<TestVector_WithoutIUnknown>::value == 2);
		static_assert(mpl::contains<TestVector_WithoutIUnknown,IClassFactory>::value);
		static_assert(!mpl::contains<TestVector_WithoutIUnknown,IUnknown>::value);
		static_assert(mpl::contains<TestVector_WithoutIUnknown,ITypeInfo>::value);

		using TestVector2 = mpl::vector<IClassFactory2,IClassFactory,IDispatch>;
		using TestVector2_WithoutIClassFactory = typename mpl::remove_if<TestVector2, is_strict_base_of<IClassFactory2>>::type;
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
	}

}  // namespace core::com