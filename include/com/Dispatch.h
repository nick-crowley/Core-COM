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
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.COM.h"
#include "com/Implements.h"
#include "com/Function.h"
#include "com/SharedPtr.h"
#include "com/Annotations.h"
#include "com/TypeLibrary.h"
#include "com/CoClassTraits.h"
#include "com/LibraryTraits.h"
#include "nstd/OutPtr.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
    template <meta::DispInterface... Interfaces>
	class Dispatch : public implements<Interfaces...,::IDispatch,::IUnknown>
	{
        static_assert(sizeof...(Interfaces) == 1, 
            "Currently IDispatch implementation is limited to only 1 interface");

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    private:
        using interface_t = nstd::tuple_front_t<std::tuple<Interfaces...>>;

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    private:
		shared_ptr<::ITypeInfo> m_typeInfo;
        
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
        template <meta::CoreCoClass CoClass>
        Dispatch(CoClass const*) 
          : m_typeInfo{TypeLibrary<CoClass>{}.typeInfo(guid_v<interface_t>)}
		{
        }
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        satisfies(Dispatch, 
			NotCopyable,
			NotEqualityComparable,
			NotSortable
        );
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        ::HRESULT 
        COMAPI GetTypeInfoCount(retval_t<::UINT> pCount) override
        {
            *pCount = 1;
            return S_OK;
        }
        
        ::HRESULT 
        COMAPI GetTypeInfo(::UINT                 iTInfo, 
                              ::LCID                 lcid, 
                              retval_t<::ITypeInfo*> ppv) override
        {
            if (iTInfo == 0) {
                return E_INVALIDARG;
            }

            return this->m_typeInfo->QueryInterface(guid_v<::ITypeInfo>, std::out_ptr(*ppv));
        }
        
        ::HRESULT
        COMAPI GetIDsOfNames(::GUID const&      riid, 
                                in_t<wchar_t*>     rgszNames, 
                                ::UINT             cNames, 
                                ::LCID             lcid, 
                                retval_t<::DISPID> rgDispId) override
        {
            return this->m_typeInfo->GetIDsOfNames(rgszNames,cNames,rgDispId);
        }
        
        ::HRESULT 
        COMAPI Invoke(::DISPID            dispIdMember, 
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
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::detail::testing
{
	// Verify class concept
	/*MIDL_INTERFACE("8F33278E-EF80-42E2-8C90-1749DBCD7836") 
	ValidCoClass : com::implements<IUnknown> {
		using library_type = ValidCoLibrary;

		LiteralString constexpr
		static class_name = L"ValidCoClass";
	};*/
	//static_assert(std::same_as<Dispatch<ValidCoClass>, Dispatch<ValidCoClass>>);

}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o