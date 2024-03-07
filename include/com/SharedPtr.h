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
#include "com/Guid.h"
#include "com/Function.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	template <meta::ComInterface Interface>
	class shared_ptr
	{
		template <meta::ComInterface> 
		friend class shared_ptr;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using type = shared_ptr<Interface>;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		Interface* Object = nullptr;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		explicit
		shared_ptr(Interface* ptr) noexcept 
		  : Object{ptr}
		{
			if (this->Object)
				this->Object->AddRef();
		}
		
		shared_ptr(meta::adopt_t, Interface* ptr) noexcept 
		  : Object{ptr}
		{
		}
		
		// DUP: std::out_ptr() supports appending, not prepending, arguments
		shared_ptr(Interface* ptr, meta::adopt_t) noexcept 
		  : shared_ptr{adopt,ptr}
		{}
		
		template <meta::ComInterface Other> 
		explicit
		shared_ptr(shared_ptr<Other> const& r) noexcept 
		{
			if (r.Object)
				r.Object->QueryInterface(__uuidof(Interface), std::out_ptr<Interface*>(this->Object));
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(shared_ptr,
			IsDefaultConstructible noexcept,
			IsEqualityComparable noexcept
		);

		shared_ptr(type const& r) noexcept 
		  : shared_ptr{r.Object}
		{
		}

		shared_ptr(type&& r) noexcept 
			: Object{std::exchange(r.Object,nullptr)}
		{
		}

		type& 
		operator=(type const& r) noexcept
		{
			type{r}.swap(*this);
			return *this;
		}
		
		type& 
		operator=(Interface* ptr) noexcept
		{
			type{ptr}.swap(*this);
			return *this;
		}

		type& 
		operator=(type&& r) noexcept
		{
			r.swap(*this);
			return *this;
		}

		~shared_ptr() noexcept
		{
			if (this->Object)
				this->Object->Release();
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		bool
		empty() const noexcept
		{
			return this->Object == nullptr;
		}

		Interface*
		operator->() const noexcept
		{
			return this->Object;
		}
		
		Interface&
		operator*() const noexcept
		{
			return *this->Object;
		}
		
		implicit operator 
		Interface*() const noexcept
		{
			return this->Object;
		}

		explicit operator 
		bool() const noexcept
		{
			return this->Object != nullptr;
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Detach and return the object pointer, leaving this object empty
		*/
		Interface*
		detach() noexcept {
			return std::exchange(this->Object, nullptr);
		}

		void
		reset() noexcept {
			type{}.swap(*this);
		}
		
		void
		reset(Interface* ptr) noexcept {
			type{ptr}.swap(*this);
		}
		
		void
		reset(meta::adopt_t, Interface* ptr) noexcept {
			type{ptr,adopt}.swap(*this);
		}
		
		// DUP: std::out_ptr() supports appending, not prepending, arguments
		void
		reset(Interface* ptr, meta::adopt_t) noexcept {
			type{ptr,adopt}.swap(*this);
		}
		
		void 
		swap(type& r) noexcept
		{
			std::swap(this->Object, r.Object);
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	template <meta::ComInterface Interface>
	bool 
	empty(shared_ptr<Interface> const& ptr)
	{
		return ptr.empty();
	}

	template <meta::ComInterface Interface, ::GUID const& ClassID>
	shared_ptr<Interface> 
	make_shared(DWORD context = CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER)
	{
		shared_ptr<Interface> object;
		win::ThrowingHResult hr = ::CoCreateInstance(ClassID, nullptr, context, guid_v<Interface>, std::out_ptr(object,adopt));
		return object;
	}
	
	template <meta::ComInterface Interface, meta::CoClassOf<Interface> CoImpl>
	shared_ptr<Interface> 
	make_shared(auto&&... args)
	{
		return shared_ptr<Interface>{
			new CoImpl{std::forward<decltype(args)>(args)...}
		};
	}
	
	template <meta::ComInterface Interface, meta::ComInterface Original>
	shared_ptr<Interface> 
	dynamic_pointer_cast(shared_ptr<Original> const& ptr)
	{
		return shared_ptr<Interface>{ptr};
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o