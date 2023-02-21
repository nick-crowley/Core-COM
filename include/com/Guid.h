#pragma once
#include "library/core.COM.h"
#include "com/BinaryString.h"
#include "core/ZString.h"

namespace core::com
{
	class ComExport Guid
	{   
    public:
        //! @brief  Generate GUID from string representation (adapted from boost::uuid)
        template <meta::Character Character>
        class Parser {
            using value_type = Character;

        public:
            satisfies(Parser,
                constexpr NotDefaultConstructible
            );

        public:
            template <std::input_iterator CharIterator>
            Guid constexpr
            static fromString(CharIterator begin, CharIterator end) 
            {
                // check open brace
                value_type c = Parser::getNextChar(begin, end);
                bool const has_open_brace = Parser::isOpenBrace(c);
                value_type const open_brace_char = c;
                if (has_open_brace) 
                    c = Parser::getNextChar(begin, end);

                bool has_dashes = false;

                uint8_t result[16] {};
                int i = 0;
                CharIterator pIn = begin;
                for (auto pOut = std::begin(result); pOut != std::end(result); ++pOut, ++i) {
                    if (pOut != std::begin(result)) 
                        c = Parser::getNextChar(pIn, end);
            
                    if (i == 4) {
                        has_dashes = Parser::isDash(c);
                        if (has_dashes) 
                            c = Parser::getNextChar(pIn, end);
                    }
            
                    // if there are dashes, they must be in every slot
                    else if (i == 6 || i == 8 || i == 10) {
                        if (has_dashes == true) {
                            if (Parser::isDash(c)) 
                                c = Parser::getNextChar(pIn, end);
                            else
                                ThrowInvalidArg(str,"Invalid GUID syntax");
                        }
                    }

                    *pOut = static_cast<uint8_t>(Parser::getValue(c));

                    c = Parser::getNextChar(pIn, end);
                    *pOut <<= 4;
                    *pOut |= Parser::getValue(c);
                }

                // check close brace
                if (has_open_brace) {
                    c = Parser::getNextChar(pIn, end);
                    Parser::checkCloseBrace(c, open_brace_char);
                }

                // check end of string - any additional data is an invalid uuid
                if (pIn != end) 
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
            template <std::input_iterator CharIterator>
            typename std::iterator_traits<CharIterator>::value_type constexpr
            static getNextChar(CharIterator& begin, CharIterator const end) {
                if (begin == end) 
                    ThrowInvalidArg(str,"GUID string too short");
                
                return *begin++;
            }
            
            std::make_unsigned_t<Character> constexpr
            static getValue(Character const c) {
                Character const digits[] {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','A','B','C','D','E','F'};
                std::make_unsigned_t<Character> const values[] { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,10,11,12,13,14,15 };
                static_assert(lengthof(digits) == lengthof(values));

                size_t const pos = ranges::find(digits, c) - digits;
                if (pos >= lengthof(digits)) 
                    ThrowInvalidArg(str,"GUID contains invalid characters");
                
                return values[pos];
            }
            
            bool constexpr
            static isDash(Character const c) {
                return c == '-';
            }
            
            bool constexpr
            static isOpenBrace(Character const c) {
                return c == '{';
            }
            
            void constexpr
            static checkCloseBrace(Character const c, Character const open_brace) {
                if (open_brace != '{' || c != '}') 
                    ThrowInvalidArg(str,"GUID missing closing brace");
            }
        };

	private:
		using type = Guid;
		using reference = type&;

	public:
        ::GUID  Value {};

    public:
		constexpr 
		explicit 
		Guid(::GUID const& g) noexcept
          : Value{g}
        {}
		
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

    public:
		Guid 
		static fromProgId(std::wstring_view str);

        Guid constexpr
		static fromString(std::string_view str) {
            return Guid::Parser<char>::fromString(str.begin(), str.end());
        }

        Guid constexpr
		static fromString(std::wstring_view str) {
            return Guid::Parser<wchar_t>::fromString(str.begin(), str.end());
        }
        
		Guid 
		static generate();
		
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
	};

	std::string
	ComExport to_string(Guid const& g);

	std::wstring
	ComExport to_wstring(Guid const&);

	template <typename T>
	Guid const
	inline guid_v { __uuidof(T) };

	namespace literals
	{
        template <ZString<char> Buffer>
		com::Guid constexpr
		ComExport operator""_guid() {
            return Guid::Parser<char>::fromString(Buffer.Text, Buffer.Text+Buffer.Length);
        }
	}
}

std::string 
ComExport to_string(::GUID const& g);

std::wstring 
ComExport to_wstring(::GUID const& g);

bool constexpr
ComExport operator==(::GUID const& l, core::com::Guid const& r) noexcept {
    return core::com::Guid{l} == r;
}

bool constexpr
ComExport operator!=(::GUID const& l, core::com::Guid const& r) noexcept {
    return core::com::Guid{l} != r;
}

bool constexpr
ComExport operator==(core::com::Guid const& l, ::GUID const& r) noexcept {
    return l == core::com::Guid{r};
}

bool constexpr
ComExport operator!=(core::com::Guid const& l, ::GUID const& r) noexcept {
    return l != core::com::Guid{r};
}