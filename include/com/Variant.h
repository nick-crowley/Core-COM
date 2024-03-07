/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2024, Nick Crowley. All rights reserved.
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
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

//! @brief	@c ::VARENUM is a bitflag
//metadata bool core::meta::Settings<core::bitwise_enum, ::VARENUM> = true;

//! @todo @c ::VARENUM isn't a bitflag; change this as @c false and define values (can we do this by range?)

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Value of dynamic runtime type (ie. COM VARIANT)
	*/
	class variant {
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		using type = variant;

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    private:
        ::VARIANT  Value { VT_EMPTY };
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        //! @todo  Support construction as VT_DATE format (ie. Variant date). This format has day-accuracy and an epoch of 1st Jan 1900. 
        //! @see   https://learn.microsoft.com/en-us/windows/win32/api/oleauto/nf-oleauto-varianttimetodosdatetime
        
        //! @brief Construct empty
        variant() noexcept = default;
        
        //! @brief Construct null
        variant(std::nullptr_t) noexcept 
          : Value{VT_NULL}
        {}
        
        //! @brief Copy-construct
        variant(type const& r) {
            this->assign(r.Value);
        }

        //! @brief Move-construct
        variant(type&& r) noexcept {
            std::memcpy(&this->Value, &r.Value, sizeof(::VARIANT));
            std::memset(&r.Value, 0, sizeof(::VARIANT));
        }
        
#pragma region Custom constructors
        //! @brief Construct from @c ::VARIANT
        explicit
        variant(::VARIANT const& r)
        {
            this->assign(r);
        }

        //! @brief Construct from @c ::VARIANT* (construct empty if @c nullptr)
        explicit
        variant(::VARIANT const* r)
        {
            if (!r) 
                return;

            this->assign(*r);
        }
        
        //! @brief Construct as @c VT_BOOL
        explicit
        variant(bool value) noexcept
          : Value{VT_BOOL}
        {
            this->Value.boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;
        }
        
        //! @brief Construct as @c VT_I1
        explicit
        variant(char value) noexcept
          : Value{VT_I1}
        {
            this->Value.cVal = value;
        }
        
        //! @brief Construct as @c VT_UI1
        explicit
        variant(unsigned char value) noexcept
          : Value{VT_UI1}
        {
            this->Value.bVal = value;
        }
        
        //! @brief Construct as @c VT_I2
        explicit
        variant(short value) noexcept
          : Value{VT_I2}
        {
            this->Value.iVal = value;
        }
        
        //! @brief Construct as @c VT_UI2
        explicit
        variant(unsigned short value) noexcept
          : Value{VT_UI2}
        {
            this->Value.uiVal = value;
        }
        
        //! @brief Construct as @c VT_INT
        explicit
        variant(int value) noexcept
          : Value{VT_INT}
        {
            this->Value.intVal = value;
        }
        
        //! @brief Construct as @c VT_UINT
        explicit
        variant(unsigned int value) noexcept
          : Value{VT_UINT}
        {
            this->Value.uintVal = value;
        }
        
        //! @brief Construct as @c VT_I4
        explicit
        variant(long value) noexcept
          : Value{VT_I4}
        {
            this->Value.lVal = value;
        }
        
        //! @brief Construct as @c VT_UI4
        explicit
        variant(unsigned long value) noexcept
          : Value{VT_UI4}
        {
            this->Value.ulVal = value;
        }
        
#if (_WIN32_WINNT >= 0x0501)
        //! @brief Construct as @c VT_I8
        explicit
        variant(long long value) noexcept
          : Value{VT_I8}
        {
            this->Value.llVal = value;
        }
        
        //! @brief Construct as @c VT_UI8
        explicit
        variant(unsigned long long value) noexcept
          : Value{VT_UI8}
        {
            this->Value.ullVal = value;
        }
#endif
        
        //! @brief Construct as @c VT_DECIMAL (128-bit integer)
        explicit
        variant(::DECIMAL const& value) noexcept
        {
            // Order is important here! Setting V_DECIMAL wipes out the entire VARIANT
            this->Value.decVal = value;
            this->Value.vt = VT_DECIMAL;
        }

        //! @brief Construct as @c VT_R4
        explicit
        variant(float value) noexcept
          : Value{VT_R4}
        {
            this->Value.fltVal = value;
        }

        //! @brief Construct as VT_R8
        explicit
        variant(double value) noexcept
          : Value{VT_R8}
        {
            this->Value.dblVal = value;
        }
        
        //! @brief Construct as @c VT_ERROR
        //! @remarks @c VT_HRESULT appears to valid in fewer contexts than @c VT_ERROR
        //! @see https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-oaut/3fe7db9f-5803-4dc4-9d14-5425d3f5461f
        explicit
        variant(win::HResult hr) noexcept
          : Value{VT_ERROR}
        {
            this->Value.scode = hr.code();
        }

        //! @brief Construct as @c VT_CY (ie. currency)
        explicit
        variant(::CY value) noexcept
          : Value{VT_CY}
        {
            this->Value.cyVal = value;
        }
        
        //! @brief  Construct as @c VT_BSTR
        //! 
        //! @param  str  Source string
        //! @param  page Source code-page
        //! 
        //! @throws std::bad_alloc          Failed to allocate memory
        //! @throws std::invalid_argument   Missing argument
        explicit
        variant(wchar_t const* wstr)
            : Value{VT_BSTR}
        {
            this->Value.bstrVal = BinaryString{ThrowIfNull(wstr)}.detach();
        }

        //! @brief  Construct as @c VT_BSTR
        //! 
        //! @param  str  Source string
        //! @param  page Source code-page
        //! 
        //! @throws std::bad_alloc          Failed to allocate memory
        //! @throws std::invalid_argument   Missing or invalid argument
        explicit
        variant(char const* str, CodePage page = CodePage::Latin1)
            : Value{VT_BSTR}
        {
            this->Value.bstrVal = BinaryString{core::widen(ThrowIfNull(str), page)}.detach();
        }
        
        //! @brief Construct as @c VT_UNKNOWN
        //! 
        //! @throws std::invalid_argument   Missing argument
        explicit
        variant(::IUnknown* r) 
          : Value{VT_UNKNOWN}
        {
            this->Value.punkVal = ThrowIfNull(r);
            r->AddRef();
        }
        
        //! @brief Construct as @c VT_DISPATCH
        //! 
        //! @throws std::invalid_argument   Missing argument
        explicit
        variant(::IDispatch* r) 
          : Value{VT_DISPATCH}
        {
            this->Value.pdispVal = ThrowIfNull(r);
            r->AddRef();
        }
#pragma endregion

        ~variant() noexcept
        {
            try {
                if (!this->empty())
                    this->clear();
            }
            catch (std::exception const& e) {
                clog << e;
            }
        }
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
#pragma region Assignment operators
        //! @brief  Assign/replace content with another variant
        type&
		operator=(type const& newValue) noexcept
        {
            type tmp{newValue};
            this->swap(tmp);
            return *this;
        }
        
        //! @brief  Assign/replace content with @c ::VARIANT
        type&
		operator=(::VARIANT const& newValue) noexcept
        {
            type tmp{newValue};
            this->swap(tmp);
            return *this;
        }
        
        //! @brief  Assign/replace content with @c bool
        type&
		operator=(bool newValue) noexcept
        {
            if (this->kind() == VT_BOOL)
                this->Value.boolVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with @c char
        type&
		operator=(char newValue) noexcept
        {
            if (this->kind() == VT_I1)
                this->Value.cVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with <tt>unsigned char</tt>
        type&
		operator=(unsigned char newValue) noexcept
        {
            if (this->kind() == VT_UI1)
                this->Value.bVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with @c short
        type&
		operator=(short newValue) noexcept
        {
            if (this->kind() == VT_I2)
                this->Value.iVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with <tt>unsigned short</tt>
        type&
		operator=(unsigned short newValue) noexcept
        {
            if (this->kind() == VT_UI2)
                this->Value.uiVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with @c int
        type&
		operator=(int newValue) noexcept
        {
            if (this->kind() == VT_INT)
                this->Value.intVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with <tt>unsigned int</tt>
        type&
		operator=(unsigned int newValue) noexcept
        {
            if (this->kind() == VT_UINT)
                this->Value.uintVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with @c long
        type&
		operator=(long newValue) noexcept
        {
            if (this->kind() == VT_I4)
                this->Value.lVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with <tt>unsigned long</tt>
        type&
		operator=(unsigned long newValue) noexcept
        {
            if (this->kind() == VT_UI4)
                this->Value.ulVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
#if (_WIN32_WINNT >= 0x0501)
        //! @brief  Assign/replace content with <tt>long long</tt>
        type&
		operator=(long long newValue) noexcept
        {
            if (this->kind() == VT_I8)
                this->Value.llVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with <tt>unsigned long long</tt>
        type&
		operator=(unsigned long long newValue) noexcept
        {
            if (this->kind() == VT_UI8)
                this->Value.ullVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
#endif
        
        //! @brief  Assign/replace content with @c float
        type&
		operator=(float newValue) noexcept
        {
            if (this->kind() == VT_R4)
                this->Value.fltVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with @c double
        type&
		operator=(double newValue) noexcept
        {
            if (this->kind() == VT_R8)
                this->Value.dblVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with currency
        type&
		operator=(::CY newValue) noexcept
        {
            if (this->kind() == VT_CY)
                this->Value.cyVal = newValue;
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with 128-bit decimal
        type&
		operator=(::DECIMAL newValue) noexcept
        {
            if (this->kind() == VT_DECIMAL) {
                this->Value.decVal = newValue;
                this->Value.vt = VT_DECIMAL;   // .decVal overwrites entire VARIANT
            }
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with @c win::HResult
        type&
		operator=(win::HResult newValue) noexcept
        {
            if (this->kind() == VT_ERROR)
                this->Value.scode = newValue.code();
            else {
                type tmp{newValue};
                this->swap(tmp);
            }
            return *this;
        }
        
        //! @brief  Assign/replace content with narrow-char string
        //! 
        //! @throws std::bad_alloc          Failed to allocate memory
        //! @throws std::invalid_argument   Missing argument
        type&
		operator=(char const* newValue)
        {
            type tmp{ThrowIfNull(newValue)};
            this->swap(tmp);
            return *this;
        }
        
        //! @brief  Assign/replace content with wide-char string
        //! 
        //! @throws std::bad_alloc          Failed to allocate memory
        //! @throws std::invalid_argument   Missing argument
        type&
		operator=(wchar_t const* newValue)
        {
            type tmp{ThrowIfNull(newValue)};
            this->swap(tmp);
            return *this;
        }
        
        //! @brief  Assign/replace content with @c IUnknown
        type&
		operator=(::IUnknown* newValue)
        {
            type tmp{ThrowIfNull(newValue)};
            this->swap(tmp);
            return *this;
        }
        
        //! @brief  Assign/replace content with @c IDispatch
        type&
		operator=(::IDispatch* newValue)
        {
            type tmp{ThrowIfNull(newValue)};
            this->swap(tmp);
            return *this;
        }
#pragma endregion
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        //! @brief  Coerce value into another type
        //! 
        //! @throws std::bad_alloc         Failed to allocate memory
        //! @throws std::invalid_argument  Variant is corrupt
        //! @throws std::range_error       Value cannot be represented within destination type
        //! @throws std::logic_error       Type cannot be converted to destination type
        //! @throws std::system_error      Operation failed
        template <::VARTYPE To>
        variant
        static cast(variant const& v) {
            variant r;
            if (win::HResult hr = ::VariantChangeType(&r.Value, &v.Value, VARIANT_ALPHABOOL, To); hr == E_OUTOFMEMORY)
                throw std::bad_alloc{};
            else if (hr == DISP_E_BADVARTYPE)
                ThrowInvalidArg(v, "Variant has invalid runtime type {:#x}", v.Value.vt);
            else if (hr == DISP_E_OVERFLOW)
                throw range_error{"{} cannot be represented as {}", v.str(), To};
            else if (hr == DISP_E_TYPEMISMATCH)
                throw logic_error{"Cannot cast from {} to {}", v.kind(), To};
            else if (!hr)
                hr.throwAlways("VariantChangeType() failed");
            return r;
        }
        
        //! @brief  Validate a runtime type descriptor
        bool
        static valid(::VARENUM rt) noexcept {
            return core::is_valid_enumerator(static_cast<::VARENUM>(rt & VT_TYPEMASK));
        }
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
        bool
        empty() const noexcept {
            return this->Value.vt == VT_EMPTY;
        }
        
        ::VARENUM
        kind() const noexcept {
            return static_cast<::VARENUM>(this->Value.vt);
        }
        
        std::string
        str() const
        {
            switch (this->kind()) {
            case VT_EMPTY:    return "Empty";
            case VT_NULL:     return "Null";
            case VT_BOOL:     return this->Value.boolVal ? "True" : "False";
            case VT_I1:       return std::format("{:d}", this->Value.cVal);
            case VT_UI1:      return std::format("{}", this->Value.bVal);
            case VT_I2:       return std::format("{}", this->Value.iVal);
            case VT_UI2:      return std::format("{}", this->Value.uiVal);
            case VT_I4:       return std::format("{}", this->Value.lVal);
            case VT_UI4:      return std::format("{}", this->Value.ulVal);
            case VT_INT:      return std::format("{}", this->Value.intVal);
            case VT_UINT:     return std::format("{}", this->Value.uintVal);
#if (_WIN32_WINNT >= 0x0501)
            case VT_I8:       return std::format("{}", this->Value.llVal);
            case VT_UI8:      return std::format("{}", this->Value.ullVal);
#endif
            case VT_R4:       return std::format("{}", this->Value.fltVal);
            case VT_R8:       return std::format("{}", this->Value.dblVal);
            
            case VT_CY:       ThrowNotImplemented("String representation of VT_CY");
            case VT_DECIMAL:  ThrowNotImplemented("String representation of VT_DECIMAL");
            
            case VT_BSTR:     return narrow(this->Value.bstrVal, CodePage::Latin1);
            case VT_ERROR:    return std::format("{:x}", this->Value.scode);
            case VT_DATE:     ThrowNotImplemented("String representation of VT_DATE");

            case VT_DISPATCH: return "IUnknown";
            case VT_UNKNOWN:  return "IDispatch";
            
            default:
                throw logic_error{"Invalid runtime type {:#x}", this->kind()};
            }
        }
        
        std::wstring
        wstr() const
        {
            if (this->kind() == VT_BSTR)
                return this->Value.bstrVal;
            
            return core::cwiden(this->str());
        }

        bool
        valid() const noexcept
        {
            return type::valid(this->kind());
        }

#pragma region Conversion operators
        //! @brief  Extract value or convert to @c bool
        explicit operator 
        bool() const
        {
            if (this->kind() == VT_BOOL)
                return this->Value.boolVal;

            return static_cast<bool>(type::cast<VT_BOOL>(*this));
        }
        
        //! @brief  Extract value or convert to @c char
        implicit operator
        char() const
        {
            if (this->kind() == VT_I1)
                return this->Value.cVal;

            return type::cast<VT_I1>(*this);
        }

        //! @brief  Extract value or convert to <tt>unsigned char</tt>
        implicit operator
        unsigned char() const
        {
            if (this->kind() == VT_UI1)
                return this->Value.bVal;

            return type::cast<VT_UI1>(*this);
        }

        //! @brief  Extract value or convert to @c short
        implicit operator
        short() const
        {
            if (this->kind() == VT_I2)
                return this->Value.iVal;

            return type::cast<VT_I2>(*this);
        }
        
        //! @brief  Extract value or convert to <tt>unsigned short</tt>
        implicit operator
        unsigned short() const
        {
            if (this->kind() == VT_UI2)
                return this->Value.uiVal;

            return type::cast<VT_UI2>(*this);
        }
        
        //! @brief  Extract value or convert to @c int
        implicit operator
        int() const
        {
            if (this->kind() == VT_INT)
                return this->Value.intVal;

            return type::cast<VT_INT>(*this);
        }
        
        //! @brief  Extract value or convert to <tt>unsigned int</tt>
        implicit operator
        unsigned int() const
        {
            if (this->kind() == VT_UINT)
                return this->Value.uintVal;

            return type::cast<VT_UINT>(*this);
        }

        //! @brief  Extract value or convert to @c long
        implicit operator
        long() const
        {
            if (this->kind() == VT_I4)
                return this->Value.lVal;

            return type::cast<VT_I4>(*this);
        }
        
        //! @brief  Extract value or convert to <tt>unsigned long</tt>
        implicit operator
        unsigned long() const
        {
            if (this->kind() == VT_UI4)
                return this->Value.ulVal;

            return type::cast<VT_UI4>(*this);
        }

#if (_WIN32_WINNT >= 0x0501)
        //! @brief  Extract value or convert to <tt>long long</tt>
        implicit operator
        long long() const
        {
            if (this->kind() == VT_I8)
                return this->Value.llVal;

            return type::cast<VT_I8>(*this);
        }
        
        //! @brief  Extract value or convert to <tt>unsigned long long</tt>
        implicit operator
        unsigned long long() const
        {
            if (this->kind() == VT_UI8)
                return this->Value.ullVal;

            return type::cast<VT_UI8>(*this);
        }
#endif
        //! @brief  Extract value or convert to @c float
        implicit operator
        float() const
        {
            if (this->kind() == VT_R4)
                return this->Value.fltVal;

            return type::cast<VT_R4>(*this);
        }
        
        //! @brief  Extract value or convert to @c double
        implicit operator
        double() const
        {
            if (this->kind() == VT_R8)
                return this->Value.dblVal;

            return type::cast<VT_R8>(*this);
        }
        
        //! @brief  Extract value or convert to @c CY
        implicit operator
        ::CY() const
        {
            if (this->kind() == VT_CY)
                return this->Value.cyVal;

            return type::cast<VT_CY>(*this);
        }
        
        //! @brief  Extract value or convert to @c DECIMAL
        implicit operator
        ::DECIMAL() const
        {
            if (this->kind() == VT_DECIMAL)
                return this->Value.decVal;

            return type::cast<VT_DECIMAL>(*this);
        }
        
        //! @brief  Extract value or convert to @c win::HResult
        implicit operator
        win::HResult() const
        {
            if (this->kind() == VT_ERROR)
                return this->Value.scode;

            return type::cast<VT_ERROR>(*this);
        }
        
        //! @brief  Copy value or generate string representation
        implicit operator
        BinaryString() const
        {
            if (this->kind() == VT_BSTR)
                return BinaryString{this->Value.bstrVal};
            
            return BinaryString{adopt, type::cast<VT_BSTR>(*this).detach().bstrVal};
        }

        //! @brief  Extract value or convert to @c IUnknown
        implicit operator
        ::IUnknown*() const
        {
            if (this->kind() == VT_UNKNOWN) {
                this->Value.punkVal->AddRef();
                return this->Value.punkVal;
            }

            return type::cast<VT_UNKNOWN>(*this);
        }

        //! @brief  Extract value or convert to @c IDispatch
        implicit operator
        ::IDispatch*() const
        {
            if (this->kind() == VT_DISPATCH) {
                this->Value.pdispVal->AddRef();
                return this->Value.pdispVal;
            }

            return type::cast<VT_DISPATCH>(*this);
        }

        //! @brief  Extract value or convert to @c VARIANT
        implicit operator
        ::VARIANT const&() const
        {
            return this->Value;
        }
#pragma endregion

        //! @brief Equality operator
        //! 
        //! @throws std::invalid_argument  Invalid runtime type
        bool 
        operator==(variant const& r) const {
            return this->equal(&r.Value);
        }

        //! @brief Equality operator for ::VARIANT
        //! 
        //! @throws std::invalid_argument  Invalid runtime type
        bool 
        operator==(::VARIANT const& r) const {
            return this->equal(&r);
        }

    private:
        //! @brief Equality operator
        //! 
        //! @throws std::invalid_argument  Missing argument -or- invalid runtime type
        bool
        equal(::VARIANT const& r) const
        {
            if (&r == &this->Value)
                return true;

            // Variants not equal if types don't match
            if (this->kind() != r.vt)
                return false;

            // Check type specific values
            switch (r.vt) {
            case VT_EMPTY:
            case VT_NULL:
                return true;
                
            case VT_BOOL:     return this->Value.boolVal == r.boolVal;
            case VT_I1:       return this->Value.cVal == r.cVal;
            case VT_UI1:      return this->Value.bVal == r.bVal;
            case VT_I2:       return this->Value.iVal == r.iVal;
            case VT_UI2:      return this->Value.uiVal == r.uiVal;
            case VT_I4:       return this->Value.lVal == r.lVal;
            case VT_UI4:      return this->Value.ulVal == r.ulVal;
            case VT_INT:      return this->Value.intVal == r.intVal;
            case VT_UINT:     return this->Value.uintVal == r.uintVal;
#if (_WIN32_WINNT >= 0x0501)
            case VT_I8:       return this->Value.llVal == r.llVal;
            case VT_UI8:      return this->Value.ullVal == r.ullVal;
#endif
            case VT_R4:       return this->Value.fltVal == r.fltVal;
            case VT_R8:       return this->Value.dblVal == r.dblVal;
            case VT_DECIMAL:  return !std::memcmp(&this->Value.decVal, &r.decVal, sizeof(::DECIMAL));
            
            case VT_BSTR:     return !std::wcscmp(this->Value.bstrVal, r.bstrVal);
            case VT_CY:       return !std::memcmp(&this->Value.cyVal, &r.cyVal, sizeof(::CY));
            case VT_DATE:     return this->Value.date == r.date;
            case VT_ERROR:    return this->Value.scode == r.scode;

            case VT_DISPATCH: return this->Value.punkVal == r.punkVal;
            case VT_UNKNOWN:  return this->Value.pdispVal == r.pdispVal;
            
            default:
                ThrowInvalidArg(r, "Invalid runtime type {:#x}", r.vt);
            }
        }
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        //! @brief Assign/overwrite content
        //! 
        //! @throws std::bad_alloc        Failed to allocate memory
        //! @throws std::invalid_argument Argument is corrupt -or- contains array which is locked
        //! @throws std::logic_error      @c *this contains an array which is current locked
        //! @throws std::runtime_error    @c *this has invalid runtime type
        //! @throws std::system_error     Operation failed
        void
        assign(::VARIANT const& r) 
        {
            if (!this->empty())
                this->clear();

            if (win::HResult hr = ::VariantCopy(&this->Value, const_cast<::VARIANT*>(&r)); hr == E_OUTOFMEMORY)
                throw std::bad_alloc{};
            else if (hr == DISP_E_ARRAYISLOCKED)
                ThrowInvalidArg(v, "Variant contains locked array");
            else if (hr == DISP_E_BADVARTYPE)
                ThrowInvalidArg(v, "Variant has invalid runtime type {:#x}", r.vt);
            else if (!hr)
                ThrowInvalidArg(r, hr.str());
        }

        void
        attach(::VARIANT&& r)
        {
            variant tmp{std::move(r)};
            this->swap(tmp);
        }

        //! @brief Erase or release content
        //! 
        //! @throws std::logic_error    @c *this contains a locked array
        //! @throws std::runtime_error  @c *this has invalid runtime type
        //! @throws std::system_error   Operation failed
        void
        clear()
        {
            if (this->empty())
                return;

            if (win::HResult hr = ::VariantClear(&this->Value); hr == DISP_E_ARRAYISLOCKED)
                throw logic_error{"Variant contains locked array"};
            else if (hr == DISP_E_BADVARTYPE)
                throw runtime_error{"Variant has invalid runtime type {:#x}", this->Value.vt};
            else if (!hr)
                hr.throwIfError("VariantClear() failed");
        }

        ::VARIANT
        detach() noexcept
        {
            return std::move(*this);
        }

        void
        swap(type& r) noexcept {
            std::swap(this->Value, r.Value);
        }
	};

    //! @brief  Empty variant sentinel value
    variant const  nullvar;
}

namespace core {
    // Ensure @c nullvar can be used like first-class citizen
    using com::nullvar;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o