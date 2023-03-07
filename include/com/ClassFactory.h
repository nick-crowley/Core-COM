#pragma once
#include "library/core.COM.h"
#include "com/Implements.h"
#include "com/Guid.h"
#include "com/HResult.h"
#include "core/FunctionLogging.h"

namespace core::com::detail
{
	template <meta::CoImpl CoImpl, meta::ForwardSequence Interfaces>
	struct CreateInstanceImpl {
		::HRESULT
		operator()(IUnknown*, ::IID const&, void**) const = delete; 
	};

	template <meta::CoImpl CoImpl, template<typename...> typename Sequence, meta::Interface... Interfaces>
	struct CreateInstanceImpl<CoImpl,Sequence<Interfaces...>> {
		::HRESULT
		operator()(IUnknown*, ::IID const& iid, void** ppv) const
		{
			if ((... || (Guid(iid) == __uuidof(Interfaces))))
			{
				auto* inst = new CoImpl{};
				return inst->QueryInterface(iid,ppv);
			}
			*ppv = nullptr;
			return E_NOINTERFACE;
		}
	};
}

namespace core::com 
{
	template <meta::CoImpl CoImpl>
		requires meta::ForwardSequence<typename CoImpl::interfaces>
	class ClassFactory : public implements<::IClassFactory,::IUnknown>
	{	
		using type = ClassFactory<CoImpl>;
		
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
			
			// Since boost::mpl is C++03 library, we need to remove the placeholder args here
			using interfaceSeq = typename mpl::remove_if<typename CoImpl::interfaces,mpl::same_as<mpl::na>>::type;
			return hr = detail::CreateInstanceImpl<CoImpl,interfaceSeq>{}(nullptr, iid, ppv);
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
	};
}
