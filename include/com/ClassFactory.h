#pragma once
#include "library/core.COM.h"
#include "com/Implements.h"
#include "com/Guid.h"
#include "com/HResult.h"
#include "core/FunctionLogging.h"

namespace core::com::detail
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

namespace core::com 
{
	template <meta::CoreCoClass CoClass>
	class ClassFactory : public implements<::IClassFactory,::IUnknown>
	{	
		using type = ClassFactory<CoClass>;

	public:
		satisfies(ClassFactory,
			IsDefaultConstructible noexcept,
            NotCopyable,
			NotEqualityComparable,
			NotSortable
		);
		
	public:
		::HRESULT
		__stdcall CreateInstance(::IUnknown*, ::IID const& iid, void** ppv) override
		{
			HResult hr = S_OK;
			logFunction(iid,ppv).withRetVals(hr,*ppv);

			if (iid == IID_IClassFactory) {
				*ppv = this;
				this->AddRef();
				return hr = S_OK;
			}

			return hr = type::CreateInstanceImpl(nullptr, iid, ppv, static_cast<interface_tuple_t<CoClass>*>(nullptr));
		}

		::HRESULT
		__stdcall LockServer(::BOOL lock) override
		{
			logFunction(lock);

			if (lock) {
				++com::numInstances;
			}
			else {
				--com::numInstances;
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
				auto* inst = new CoClass{};
				return inst->QueryInterface(iid,ppv);
			}

			*ppv = nullptr;
			return E_NOINTERFACE;
		}
	};
}
