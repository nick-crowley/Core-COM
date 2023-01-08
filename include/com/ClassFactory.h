#pragma once
#include "comFramework.h"
#include "com/Implements.h"
#include "com/Guid.h"

namespace com::detail
{
#if UNUSED
	template <typename InterfaceTuple>
	struct make_implements;

	template <typename... Interfaces>
	struct make_implements<std::tuple<Interfaces...>> : meta::identity<implements<Interfaces...>>
	{};
		
	template <typename InterfaceTuple>
	using make_implements_t = typename make_implements<InterfaceTuple>::type;
#endif
}

namespace com 
{
	template <meta::CoClass CoClass>
	class ClassFactory : public implements<IClassFactory>
	{	
		using type = ClassFactory<CoClass>;

	public:
		satisfies(ClassFactory,
			IsDefaultConstructible_noexcept,
			NotCopyable,
			NotEqualityComparable,
			NotSortable
		);
		
	public:
		::HRESULT
		__stdcall CreateInstance(IUnknown*, ::IID const& iid, void** ppv) override
		{
			if (iid == IID_IClassFactory) {
				*ppv = this;
				this->AddRef();
				return S_OK;
			}

			return type::CreateInstanceImpl(nullptr, iid, ppv, static_cast<interface_tuple_t<CoClass>*>(nullptr));
		}

		::HRESULT
		__stdcall LockServer(::BOOL lock) override
		{
			if (lock) {
				++g_numInstances;
			}
			else {
				--g_numInstances;
			}
			return S_OK;
		}

	private:
		template <meta::Interface... Interfaces>
		::HRESULT
		static CreateInstanceImpl(IUnknown*, ::IID const& iid, void** ppv, std::tuple<Interfaces...>* = nullptr) 
		{
			if ((... || (Guid(iid) == __uuidof(Interfaces))))
			{
				*ppv = new CoClass{};
				return S_OK;
			}

			*ppv = nullptr;
			return E_NOINTERFACE;
		}
	};
}
