#pragma once
#include "library/core.COM.h"
#include "meta/Concepts.h"
#include "com/HResult.h"
#include "core/FunctionLogging.h"

namespace core::com
{
	namespace detail
	{
		//! @brief	Realizes an arbitrary type-list of COM interfaces
		template <typename ForwardSequence>
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
	
		namespace testing 
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
		}
	
		template <typename ForwardSequence>
		struct distinct_interfaces_impl : std::type_identity_t<ForwardSequence>{};

		template <typename I>
		struct distinct_interfaces_impl<mpl::vector<I>> : std::type_identity_t<mpl::vector<I>>{};

		template <typename I, typename ...R>
		struct distinct_interfaces_impl<mpl::vector<I,R...>> : std::type_identity_t<
			typename mpl::remove_if<mpl::vector<I,R...>,is_strict_base_of<I>>::type
		>{};

		template <meta::Interface... I>
		struct distinct_interfaces : distinct_interfaces_impl<mpl::vector<I...>>
		{};

		//! @brief	Type-list of distinct COM interfaces (ie. with ancestral interfaces removed)
		//! @tparam	Interfaces...	Set of COM interfaces, possibly including common base classes
		template <meta::Interface... Interfaces>
		using distinct_interfaces_t = typename distinct_interfaces<Interfaces...>::type;
	
		namespace testing
		{
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
	}

	extern std::atomic_long g_numInstances;

	template <meta::Interface... Interfaces>
	class implements : public detail::MultipleRealization<detail::distinct_interfaces_t<Interfaces...>>
	{	
		using type = implements<Interfaces...>;

	public:
		using interface_tuple = std::tuple<Interfaces...>;

	private:
		std::atomic_long m_refCount = 0;

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
			logFunction(iid,ppv).withRetVals(hr,*ppv);

			if (!ppv) {
				return hr = E_INVALIDARG;
			}

			return hr = this->QueryInterfaceImpl<Interfaces...>(iid,ppv);
		}

		::ULONG
		__stdcall AddRef() override
		{
			logFunction().withRetVals(std::cref(this->m_refCount), std::cref(g_numInstances));

			if (this->m_refCount++ == 0) {
				++g_numInstances;
			}

			return this->m_refCount;
		}
	
		::ULONG
		__stdcall Release() override
		{
			logFunction().withRetVals(std::cref(this->m_refCount), std::cref(g_numInstances));

			if (this->m_refCount <= 0)
				clog << Warning{"Coclass has an invalid reference count of {}", this->m_refCount.load()};

			if (--this->m_refCount <= 0)
			{
				delete this;
				--g_numInstances;
				return 0;
			}

			return this->m_refCount;
		}

	private:
		template <meta::Interface Interface, meta::Interface... Remainder>
		::HRESULT
		__stdcall QueryInterfaceImpl(::IID const& iid, void** ppv) 
		{
			if (__uuidof(Interface) == iid) 
			{
				// Since coclasses commonly realize multiple interfaces, each of which inherits from IUnknown,
				// it's highly likely there will not be an unambiguous conversion to IUnknown. COM also has an
				// identity requirement stating that, whatever conversion we choose, it must be consistent; so
				// we're converting via the first interface in the list.
				if constexpr (!std::is_convertible_v<type const*, Interface const*>)
					*ppv = static_cast<Interface*>(
						static_cast<nstd::tuple_front_t<interface_tuple>*>(this)
					);
				else
					*ppv = static_cast<Interface*>(this);
				this->AddRef();
				return S_OK;
			}

			if constexpr (sizeof...(Remainder))
			{
				return this->QueryInterfaceImpl<Remainder...>(iid, ppv);
			}
			else
			{
				return E_NOINTERFACE;
			}
		}
	};
	
	template <typename T>
	using interface_tuple_t = typename T::interface_tuple;

}  // namespace core::com
