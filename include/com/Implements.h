#pragma once
#include "library/core.COM.h"
#include "meta/Concepts.h"

namespace core::com 
{
	extern std::atomic_long g_numInstances;
	
	template <meta::Interface... Interfaces>
	class implements : public Interfaces...
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
			if (!ppv) {
				return E_INVALIDARG;
			}

			return this->QueryInterfaceImpl<Interfaces...>(iid,ppv);
		}

		::ULONG
		__stdcall AddRef() override
		{
			if (this->m_refCount++ == 0) {
				++g_numInstances;
			}

			return this->m_refCount;
		}
	
		::ULONG
		__stdcall Release() override
		{
			if (--this->m_refCount == 0)
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
