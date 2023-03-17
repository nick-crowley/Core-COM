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
    /* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Generic implementation of @c IDispatch for any interface
	*/
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
        Dispatch(CoClass const&) 
          : m_typeInfo{TypeLibrary<CoClass>{}.typeInfo(guid_v<interface_t>)}
		{
        }
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        satisfies(Dispatch, 
            NotDefaultConstructible,
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
			win::HResult hr = S_OK;
			logFunctionArgs(pCount).withRetVals(hr,*pCount);

            *pCount = 1;
            return hr = S_OK;
        }
        
        ::HRESULT 
        COMAPI GetTypeInfo(::UINT                 idx, 
                           ::LCID                 locale, 
                           retval_t<::ITypeInfo*> ppv) override
        {
            win::HResult hr = S_OK;
			logFunctionArgs(idx,locale).withRetVals(hr,*ppv);

            if (idx != 0) 
                return hr = DISP_E_BADINDEX;

            return hr = this->m_typeInfo->QueryInterface(guid_v<::ITypeInfo>, std::out_ptr(*ppv));
        }
        
        ::HRESULT
        COMAPI GetIDsOfNames(::GUID const&      iid, 
                             in_t<wchar_t*>     szNames, 
                             ::UINT             cNames, 
                             ::LCID             locale, 
                             retval_t<::DISPID> arrDispId) override
        {
            win::HResult hr = S_OK;
            std::span const names{szNames,cNames};
            logFunctionArgs(iid,names,locale).withRetVals(hr,*arrDispId);

            return hr = this->m_typeInfo->GetIDsOfNames(szNames,cNames,arrDispId);
        }
        
        ::HRESULT 
        COMAPI Invoke(::DISPID            dispIdMember, 
                      ::GUID const&       iid, 
                      ::LCID              locale, 
                      ::WORD              wFlags, 
                      out_t<::DISPPARAMS> pDispParams, 
                      out_t<::VARIANT>    pVarResult, 
                      out_t<::EXCEPINFO>  pExcepInfo, 
                      out_t<::UINT>       puArgErr) override
        {
            win::HResult hr = S_OK;
            //! @todo   @c ::VARIANT, @c ::EXCEPTINFO, @c ::DISPPARAMS should model @c core::Stringable
			logFunctionArgs(dispIdMember,iid,locale,wFlags).withRetVals(hr, *puArgErr /* *pDispParams, *pVarResult, *pExcepInfo */);

            return hr = this->m_typeInfo->Invoke(static_cast<interface_t*>(this), dispIdMember, wFlags,
                pDispParams, pVarResult, pExcepInfo, puArgErr);
        }
	};
    
}	// namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o


// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o