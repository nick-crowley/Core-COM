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
#include "com/BinaryString.h"
#include "core/ZString.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
bool
ComExport operator<(::GUID const& l, ::GUID const& r);
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
    /* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Literal-type GUID with compile-time parsing
	*/
	class ComExport Guid
	{
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    private:
		using type = Guid;
		using reference = type&;
		
    public:
        //! @brief  Generate GUID from string representation (adapted from boost::uuid)
        template <std::input_iterator CharIterator>
        class GuidParser 
        {
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
        private:
            using type = GuidParser<CharIterator>;
            using character_type = typename std::iterator_traits<CharIterator>::value_type;
            
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
        private:
            CharIterator const sourceBegin;
            CharIterator const sourceEnd;
            
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
        public:
            constexpr
            GuidParser(CharIterator begin, CharIterator end)
                : sourceBegin{begin}, sourceEnd{end}
            {}
            
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
        public:
            satisfies(GuidParser,
                constexpr NotDefaultConstructible,
                constexpr IsCopyConstructible,
                constexpr IsMoveConstructible,
                NotCopyAssignable,
                NotMoveAssignable,
                NotEqualityComparable,
                NotSortable
            );
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

            // o~=~-~=~-~=~-~=~-~=~-~=~-~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o

            // o~=~-~=~-~=~-~=~-~=~-~=~-~=o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
        public:
            Guid constexpr
            parse() 
            {
                // check open brace
                CharIterator pIn = this->sourceBegin;
                character_type c = this->getNextChar(pIn);
                bool const has_open_brace = this->isOpenBrace(c);
                character_type const open_brace_char = c;
                if (has_open_brace) 
                    c = this->getNextChar(pIn);

                bool has_dashes = false;

                uint8_t result[16] {};
                int i = 0;
                for (auto pOut = std::begin(result); pOut != std::end(result); ++pOut, ++i) {
                    if (pOut != std::begin(result)) 
                        c = this->getNextChar(pIn);
            
                    if (i == 4) {
                        has_dashes = this->isDash(c);
                        if (has_dashes) 
                            c = this->getNextChar(pIn);
                    }
            
                    // if there are dashes, they must be in every slot
                    else if (i == 6 || i == 8 || i == 10) {
                        if (has_dashes == true) {
                            if (this->isDash(c)) 
                                c = this->getNextChar(pIn);
                            else
                                ThrowInvalidArg(str,"Invalid GUID syntax");
                        }
                    }

                    *pOut = static_cast<uint8_t>(this->getValue(c));

                    c = this->getNextChar(pIn);
                    *pOut <<= 4;
                    *pOut |= this->getValue(c);
                }

                // check close brace
                if (has_open_brace) {
                    c = this->getNextChar(pIn);
                    this->checkCloseBrace(c, open_brace_char);
                }

                // check end of string - any additional data is an invalid uuid
                if (pIn != this->sourceEnd) 
                    ThrowInvalidArg(str,"GUID string too long");
                
                ::GUID guid {};
                guid.Data1 = static_cast<uint32_t>(result[0]) << 24
                           | static_cast<uint32_t>(result[1]) << 16
                           | static_cast<uint32_t>(result[2]) << 8
                           | static_cast<uint32_t>(result[3]);
                guid.Data2 = static_cast<uint16_t>(result[4]) << 8 
                           | static_cast<uint16_t>(result[5]);
                guid.Data3 = static_cast<uint16_t>(result[6]) << 8 
                           | static_cast<uint16_t>(result[7]);
                std::copy(&result[0x08], &result[0x10], &guid.Data4[0]);
                return Guid{guid};
            }
    
        private:
            character_type constexpr
            getNextChar(CharIterator& pos) {
                if (pos == this->sourceEnd) 
                    ThrowInvalidArg(str,"GUID string too short");
                
                return *pos++;
            }
            
            std::make_unsigned_t<character_type> constexpr
            getValue(character_type const c) {
                character_type const digits[] {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','A','B','C','D','E','F'};
                std::make_unsigned_t<character_type> const values[] { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,10,11,12,13,14,15 };
                static_assert(lengthof(digits) == lengthof(values));

                size_t const pos = ranges::find(digits, c) - digits;
                if (pos >= lengthof(digits)) 
                    ThrowInvalidArg(str,"GUID contains invalid characters");
                
                return values[pos];
            }
            
            bool constexpr
            isDash(character_type const c) {
                return c == '-';
            }
            
            bool constexpr
            isOpenBrace(character_type const c) {
                return c == '{';
            }
            
            void constexpr
            checkCloseBrace(character_type const c, character_type const open_brace) {
                if (open_brace != '{' || c != '}') 
                    ThrowInvalidArg(str,"GUID missing closing brace");
            }
        };
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
        ::GUID  Value {};
        
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
		constexpr 
		explicit 
		Guid(::GUID const& g) noexcept
          : Value{g}
        {}
		
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
		satisfies(Guid,
			constexpr IsDefaultConstructible noexcept,
			constexpr IsCopyConstructible noexcept,
			constexpr IsCopyAssignable noexcept,
			constexpr IsDestructible noexcept,
			constexpr NotSortable
		);

		reference constexpr
		operator=(::GUID const& g) noexcept {
			this->Value = g;
			return *this;
		}
        
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
		Guid 
		static fromProgId(std::wstring_view str);

        Guid constexpr
		static fromString(std::string_view str) {
            return GuidParser{str.begin(), str.end()}.parse();
        }

        Guid constexpr
		static fromString(std::wstring_view str) {
            return GuidParser{str.begin(), str.end()}.parse();
        }
        
		Guid 
		static generate();
        
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
		wstring 
		wstr() const;
		
		bool constexpr
		operator==(type const& r) const noexcept {
            using namespace std;
            // Cannot default this operator because it would call non-constexpr ::operator==(::GUID,::GUID)
            return this->Value.Data1 == r.Value.Data1
                && this->Value.Data2 == r.Value.Data2
                && this->Value.Data3 == r.Value.Data3
                && equal(begin(this->Value.Data4), end(this->Value.Data4), begin(r.Value.Data4), end(r.Value.Data4));
        }
		
		bool constexpr
		operator!=(type const& r) const noexcept {
            using namespace std;
            return this->Value.Data1 != r.Value.Data1
                || this->Value.Data2 != r.Value.Data2
                || this->Value.Data3 != r.Value.Data3
                || !equal(begin(this->Value.Data4), end(this->Value.Data4), begin(r.Value.Data4), end(r.Value.Data4));
        }

		constexpr 
		implicit operator 
		::GUID const&() const noexcept { 
			return this->Value; 
		}

        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	};
    
	template <typename T>
	Guid const
	inline guid_v { __uuidof(T) };
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	std::string
	ComExport to_string(Guid const& g);

	std::wstring
	ComExport to_wstring(Guid const& g);
}

std::string 
ComExport to_string(::GUID const& g);

std::wstring 
ComExport to_wstring(::GUID const& g);

static_assert(core::meta::Stringable<::GUID>);

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
    namespace literals
	{
        template <ZString<char const> Buffer>
		com::Guid consteval
		operator""_guid() {
            return Guid::GuidParser{Buffer.Text, Buffer.Text+Buffer.Length}.parse();
        }
	}
}

bool constexpr
operator==(::GUID const& l, core::com::Guid const& r) noexcept {
    return core::com::Guid{l} == r;
}

bool constexpr
operator!=(::GUID const& l, core::com::Guid const& r) noexcept {
    return core::com::Guid{l} != r;
}

bool constexpr
operator==(core::com::Guid const& l, ::GUID const& r) noexcept {
    return l == core::com::Guid{r};
}

bool constexpr
operator!=(core::com::Guid const& l, ::GUID const& r) noexcept {
    return l != core::com::Guid{r};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o