#pragma once
#include "comFramework.h"
#include "meta/Concepts.h"

namespace com 
{
	extern std::atomic_long g_numInstances;
	
	template <meta::Interface... Interfaces>
	class implements : public Interfaces...
	{	
		using type = implements<Interfaces...>;

	public:
		using interface_tuple = std::tuple<Interfaces...>;

	private:
		std::atomic_long m_refCount = 1;

	public:
		satisfies(implements,
			IsDefaultConstructible_noexcept,
			NotCopyable,
			NotEqualityComparable,
			NotSortable
		);

	public:
		::HRESULT
		__stdcall QueryInterface(::IID const& id, void** out) override
		{
			if (!out) {
				return E_INVALIDARG;
			}

			return this->QueryInterfaceImpl<Interfaces...>(id,out);
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
		__stdcall QueryInterfaceImpl(::IID const& id, void** out) 
		{
			if (__uuidof(Interface) == id) 
			{
				auto* inst = static_cast<Interface*>(this);
				inst->AddRef();
				*out = inst;
				return S_OK;
			}

			if constexpr (sizeof...(Remainder))
			{
				return this->QueryInterfaceImpl<Remainder...>(id, out);
			}
			else
			{
				return E_NOINTERFACE;
			}
		}
	};
	
	template <typename T>
	using interface_tuple_t = typename T::interface_tuple;

}  // namespace com
