#pragma once
#include "library/core.COM.h"
#include "com/Implements.h"
#include "com/Function.h"
#include "com/SharedPtr.h"
#include "com/Annotations.h"
#include "com/CoClassTraits.h"
#include "com/LibraryTraits.h"
#include "nstd/OutPtr.h"

namespace core::com
{
    template <meta::DispInterface... Interfaces>
	class Dispatch : public implements<Interfaces...,::IDispatch,::IUnknown>
	{
        static_assert(sizeof...(Interfaces) == 1, 
            "Currently IDispatch implementation is limited to only 1 interface");

        using interface_t = nstd::tuple_front_t<std::tuple<Interfaces...>>;

    private:
		shared_ptr<::ITypeInfo> m_typeInfo;

	public:
        template <meta::CoreCoClass CoClass>
        Dispatch(CoClass const*) 
		{
            auto constexpr
            static loadRegTypeLib = com::function<1>(::LoadRegTypeLib);

            auto constexpr
            static getTypeInfoOfGuid = com::method<1>(&::ITypeLib::GetTypeInfoOfGuid);

            Version constexpr 
            static classVer = coclass_traits<CoClass>::class_version;

            Guid const 
            static libGuid = library_traits<CoClass::library_type>::library_guid;

            shared_ptr<::ITypeLib>  typeLibrary{ loadRegTypeLib(libGuid, classVer.Major, classVer.Minor, 0) };
            this->m_typeInfo = getTypeInfoOfGuid(*typeLibrary,__uuidof(interface_t));
        }

        satisfies(Dispatch, 
			NotCopyable,
			NotEqualityComparable,
			NotSortable
        );
        
    public:
        ::HRESULT 
        __stdcall GetTypeInfoCount(retval_t<::UINT> pCount) override
        {
            *pCount = 1;
            return S_OK;
        }
        
        ::HRESULT 
        __stdcall GetTypeInfo(::UINT                 iTInfo, 
                              ::LCID                 lcid, 
                              retval_t<::ITypeInfo*> ppv) override
        {
            if (iTInfo == 0) {
                return E_INVALIDARG;
            }

            return this->m_typeInfo->QueryInterface(__uuidof(::ITypeInfo), nstd::out_ptr<void*>(*ppv));
        }
        
        ::HRESULT
        __stdcall GetIDsOfNames(::GUID const&      riid, 
                                in_t<wchar_t*>     rgszNames, 
                                ::UINT             cNames, 
                                ::LCID             lcid, 
                                retval_t<::DISPID> rgDispId) override
        {
            return this->m_typeInfo->GetIDsOfNames(rgszNames,cNames,rgDispId);
        }
        
        ::HRESULT 
        __stdcall Invoke(::DISPID            dispIdMember, 
                         ::GUID const&       riid, 
                         ::LCID              lcid, 
                         ::WORD              wFlags, 
                         out_t<::DISPPARAMS> pDispParams, 
                         out_t<::VARIANT>    pVarResult, 
                         out_t<::EXCEPINFO>  pExcepInfo, 
                         out_t<::UINT>       puArgErr) override
        {
            return this->m_typeInfo->Invoke(static_cast<interface_t*>(this), dispIdMember, wFlags,
                pDispParams, pVarResult, pExcepInfo, puArgErr);
        }
	};
    
}	// namespace core::com
