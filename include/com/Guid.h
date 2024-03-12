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
#include "com/BasicString.h"
#include "core/LiteralString.h"
#include "core/ZString.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

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
        //! @brief  Generate string representation from GUID
        class Stringifier {
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
            using type = Stringifier;
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
            
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
            
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
        public:
            satisfies(Stringifier,
                NotDefaultConstructible,
                NotEqualityComparable,
                NotSortable
            );
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
        public:
            std::wstring constexpr
            static wstr(::GUID const& g) {
                std::wstring result;
                result.push_back('{');
                type::formatTo(g.Data1, result);
                result.push_back('-');
                type::formatTo(g.Data2, result);
                result.push_back('-');
                type::formatTo(g.Data3, result);
                result.push_back('-');
                type::formatTo(std::span{&g.Data4[0], 2}, result);
                result.push_back('-');
                type::formatTo(std::span{&g.Data4[2], 6}, result);
                result.push_back('}');
                return result;
            }

        private:
            nstd::return_t<std::wstring&> constexpr
            static formatByte(unsigned char ch, std::wstring& out)
            {
                char constexpr
		        digits[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	
		        out.push_back(digits[ch / 16]);
			    out.push_back(digits[ch % 16]);
                return out;
            }
            
            nstd::return_t<std::wstring&> constexpr
            static formatTo(unsigned short value, std::wstring& out) {
                formatByte(static_cast<unsigned char>((value >> 8) & 0xff), out);
                formatByte(static_cast<unsigned char>((value >> 0) & 0xff), out);
                return out;
            }
            
            nstd::return_t<std::wstring&> constexpr
            static formatTo(unsigned long value, std::wstring& out) {
                formatByte(static_cast<unsigned char>((value >> 24) & 0xff), out);
                formatByte(static_cast<unsigned char>((value >> 16) & 0xff), out);
                formatByte(static_cast<unsigned char>((value >> 8) & 0xff), out);
                formatByte(static_cast<unsigned char>((value >> 0) & 0xff), out);
                return out;
            }
            
            nstd::return_t<std::wstring&> constexpr
            static formatTo(std::span<unsigned char const> bytes, std::wstring& out) {
                for (auto const& b : bytes)
                    formatByte(b, out);
                return out;
            }
            // o~=~-~=~-~=~-~=~-~=~-~=~-~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
        };

        //! @brief  Generate GUID from string representation
        template <std::input_iterator CharIterator>
        class Parser 
        {
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
        private:
            using type = Parser<CharIterator>;
            using character_type = typename std::iterator_traits<CharIterator>::value_type;
            
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
        private:
            CharIterator const sourceBegin;
            CharIterator const sourceEnd;
            
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
        public:
            constexpr
            Parser(CharIterator begin, CharIterator end)
                : sourceBegin{begin}, sourceEnd{end}
            {}
            
            // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
        public:
            satisfies(Parser,
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
            //! @brief  Parse string representation (adapted from boost)
            //! @see  @c boost::uuids::string_generator::operator()
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
		explicit constexpr 
		Guid(::GUID const& g) noexcept
          : Value{g}
        {}
		
        explicit constexpr
		Guid(std::string_view str)
          : Value{Parser{str.begin(), str.end()}.parse().Value}
        {
        }

        explicit constexpr
		Guid(std::wstring_view str)
          : Value{Parser{str.begin(), str.end()}.parse().Value}
        {
        }
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

		Guid 
		static generate();
        
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
		LiteralString<wchar_t,39> constexpr
		wstr() const {
            return LiteralString<wchar_t,39>{Stringifier::wstr(this->Value).c_str()};
        }
		
		bool constexpr
		operator==(type const& r) const noexcept {
            using namespace std;
            // Cannot default this operator because it would call non-constexpr ::operator==(::GUID,::GUID)
            return this->Value.Data1 == r.Value.Data1
                && this->Value.Data2 == r.Value.Data2
                && this->Value.Data3 == r.Value.Data3
                && equal(begin(this->Value.Data4), end(this->Value.Data4), begin(r.Value.Data4), end(r.Value.Data4));
        }
		
		implicit constexpr
        operator ::GUID const&() const noexcept { 
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

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace std 
{
	template <nstd::AnyOf<char,wchar_t> Char>
	struct formatter<core::com::Guid, Char> 
    {
		auto constexpr
		parse(basic_format_parse_context<Char>& ctx) {
            return ranges::find(ctx, '}');
        }

		auto 
		format(core::com::Guid const& g, format_context& ctx) const requires std::same_as<Char,char> {
			return format_to(ctx.out(), "{}", ::to_string(g.Value));
		}
        
		auto 
		format(core::com::Guid const& g, wformat_context& ctx) const requires std::same_as<Char,wchar_t> {
			return format_to(ctx.out(), L"{}", ::to_wstring(g.Value));
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
    inline namespace literals
	{
        inline namespace guid_literals
	    {
            template <ZString<char const> Buffer>
		    com::Guid consteval
		    operator""_guid() {
                return Guid::Parser{Buffer.Text, Buffer.Text+Buffer.Length}.parse();
            }
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

bool constexpr
operator<(::GUID const& l, ::GUID const& r) noexcept {
	return l.Data1 < r.Data1
		&& l.Data2 < r.Data2
		&& l.Data3 < r.Data3
		&& std::array{l.Data4} < std::array{r.Data4};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{
    //! @test  Verify @c ::GUID models @c meta::Stringable
    static_assert(core::meta::Stringable<::GUID>);
    
    //! @test  Verify @c com::Guid models @c meta::Stringable
    static_assert(core::meta::Stringable<Guid>);
    
    //! @test  Verify @c core::com::Guid::Guid() correctly parses string representations
    static_assert(
        Guid{"{FE2C8ED8-98DA-4BF7-BD5E-0AA1D38FB5E6}"} == ::GUID{0xFE2C8ED8,0x98DA,0x4BF7,{0xBD,0x5E,0x0A,0xA1,0xD3,0x8F,0xB5,0xE6}}
    );
    static_assert(
        Guid{L"{FE2C8ED8-98DA-4BF7-BD5E-0AA1D38FB5E6}"} == ::GUID{0xFE2C8ED8,0x98DA,0x4BF7,{0xBD,0x5E,0x0A,0xA1,0xD3,0x8F,0xB5,0xE6}}
    );

    //! @test  Verify @c core::com::operator""_guid produces the correct GUID
    static_assert(
        "{42C386F4-95A0-43A7-B24C-7288D31E98C2}"_guid == ::GUID{0x42C386F4,0x95A0,0x43A7,{0xB2,0x4C,0x72,0x88,0xD3,0x1E,0x98,0xC2}}
    );

    //! @test  Verify @c core::com::Guid::wstr() produces string-representation of the correct length
    static_assert(
        "{42C386F4-95A0-43A7-B24C-7288D31E98C2}"_guid.wstr().size() == 38
    );

    //! @test  Verify @c core::com::Guid::wstr() produces correct string-representation
    static_assert(
          "{42C386F4-95A0-43A7-B24C-7288D31E98C2}"_guid.wstr() == L"{42C386F4-95A0-43A7-B24C-7288D31E98C2}"sv
    );
	
	//! @test  Verify @c std::format() supports @c core::com::Guid
	static_assert(requires { std::format("{}", "{42C386F4-95A0-43A7-B24C-7288D31E98C2}"_guid); });
	static_assert(requires { std::format(L"{}", "{42C386F4-95A0-43A7-B24C-7288D31E98C2}"_guid); });
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o