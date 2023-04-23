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
#include "library/core.Com.h"
#include "com/Adapter.h"
#include "com/Annotations.h"
#include "com/SharedPtr.h"
#include "com/CoClassTraits.h"
#include "com/LibraryTraits.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
    template <meta::CoreCoClass CoClass>
    class TypeLibrary
    {
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    private:
        class CoTypeLibraryImpl : public adapter<::ITypeLib>
        {
            using base = com::adapter<interface_type>;

        public:
            method_t<3,&interface_type::FindName>          findName;
            method_t<4,&interface_type::GetDocumentation>  getDocumentation;
            method_t<1,&interface_type::GetLibAttr>        getLibAttr;
            method_t<1,&interface_type::GetTypeComp>       getTypeComp;
            method_t<1,&interface_type::GetTypeInfo>       getTypeInfo;
#if SupportNonStandardMethodSignature
            method_t<0,&interface_type::GetTypeInfoCount>  getTypeInfoCount;
#endif
            method_t<1,&interface_type::GetTypeInfoOfGuid> getTypeInfoOfGuid;
            method_t<1,&interface_type::GetTypeInfoType>   getTypeInfoType;
            method_t<1,&interface_type::IsName>            isName;
#if SupportNonStandardMethodSignature
            method_t<0,&interface_type::ReleaseTLibAttr>   releaseTLibAttr;
#endif

        public:
            explicit
            CoTypeLibraryImpl(com::shared_ptr<interface_type> ptr) 
              : base{ptr},
                findName{this->method<3>(&interface_type::FindName)},
                getDocumentation{this->method<4>(&interface_type::GetDocumentation)},
                getLibAttr{this->method<1>(&interface_type::GetLibAttr)},
                getTypeComp{this->method<1>(&interface_type::GetTypeComp)},
                getTypeInfo{this->method<1>(&interface_type::GetTypeInfo)},
#if SupportNonStandardMethodSignature
                getTypeInfoCount{this->method<0>(&interface_type::GetTypeInfoCount)},
#endif
                getTypeInfoOfGuid{this->method<1>(&interface_type::GetTypeInfoOfGuid)},
                getTypeInfoType{this->method<1>(&interface_type::GetTypeInfoType)},
                isName{this->method<1>(&interface_type::IsName)}
#if SupportNonStandardMethodSignature
                releaseTLibAttr{this->method<0>(&interface_type::ReleaseTLibAttr)}
#endif
            {}
        };
        
        auto constexpr
        inline static loadRegTypeLib = com::function<1>(::LoadRegTypeLib);

    public:
        ::LCID constexpr
        static NeutralLocale = NULL;

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    private:
        CoTypeLibraryImpl  library;

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        explicit
        TypeLibrary(::LCID locale = NeutralLocale) 
          : library{TypeLibrary::load(locale)}
        {}

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    private:
        shared_ptr<::ITypeLib>
        static load(::LCID locale) {
            auto* const typelib = TypeLibrary::loadRegTypeLib(
                library_guid_v<coclass_library_t<CoClass>>, 
                coclass_version_v<CoClass>.Major, 
                coclass_version_v<CoClass>.Minor, 
                locale
            );
            return {typelib, adopt};
        }
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        shared_ptr<::ITypeInfo>
        typeInfo(Guid g) const {
            return {this->library.getTypeInfoOfGuid(g), adopt};
        }

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    };
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o