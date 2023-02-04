#pragma once
#include "library/core.COM.h"
#include "com/Guid.h"
#include "com/Function.h"

namespace core::com
{
	template <meta::Interface Interface>
	class shared_ptr
	{
		template <meta::Interface> 
		friend class shared_ptr;

		using type = shared_ptr<Interface>;

	private:
		Interface* m_object = nullptr;

	public:
		satisfies(shared_ptr,
			IsDefaultConstructible noexcept,
			IsEqualityComparable noexcept
		);
		
		explicit
		shared_ptr(Interface* ptr) noexcept 
		  : m_object{ptr}
		{
			if (this->m_object)
				this->m_object->AddRef();
		}
		
		explicit
		shared_ptr(meta::adopt_t, Interface* ptr) noexcept 
		  : m_object{ptr}
		{
		}
		
		// DUP: std::out_ptr() supports appending, not prepending, arguments
		explicit
		shared_ptr(Interface* ptr, meta::adopt_t) noexcept 
		  : shared_ptr{adopt,ptr}
		{}
		
		shared_ptr(type const& r) noexcept 
		  : shared_ptr{r.m_object}
		{
		}
	
		template <meta::Interface Other> 
		shared_ptr(shared_ptr<Other> const& r) noexcept 
		{
			if (r.m_object)
				r.m_object->QueryInterface(__uuidof(Interface), std::out_ptr<Interface*>(this->m_object));
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

		shared_ptr(type&& r) noexcept 
			: m_object{std::exchange(r.m_object,nullptr)}
		{
		}

		type& 
		operator=(type&& r) noexcept
		{
			r.swap(*this);
			return *this;
		}

		~shared_ptr() noexcept
		{
			if (this->m_object)
				this->m_object->Release();
		}

	public:
		bool
		empty() const
		{
			return this->m_object == nullptr;
		}
		
	public:
		void
		reset() {
			type{}.swap(*this);
		}
		
		void
		reset(Interface* ptr) {
			type{ptr}.swap(*this);
		}
		
		void
		reset(meta::adopt_t, Interface* ptr) {
			type{ptr,adopt}.swap(*this);
		}
		
		// DUP: std::out_ptr() supports appending, not prepending, arguments
		void
		reset(Interface* ptr, meta::adopt_t) {
			type{ptr,adopt}.swap(*this);
		}
		
		void 
		swap(type& r) noexcept
		{
			std::swap(this->m_object, r.m_object);
		}

		Interface*
		operator->() const noexcept
		{
			return this->m_object;
		}
		
		Interface&
		operator*() const noexcept
		{
			return *this->m_object;
		}
		
		implicit operator 
		Interface*() const noexcept
		{
			return this->m_object;
		}

		explicit operator 
		bool() const noexcept
		{
			return this->m_object != nullptr;
		}
	};
	
	template <meta::Interface Interface>
	bool 
	empty(shared_ptr<Interface> const& ptr)
	{
		return ptr.empty();
	}

	template <::GUID const& ClassID, meta::Interface Interface>
	auto 
	make_shared(DWORD context = CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER)
	{
		shared_ptr<Interface> object;
		ThrowingHResult hr = ::CoCreateInstance(ClassID, nullptr, context, guid_v<Interface>, std::out_ptr<Interface*>(object,adopt));
		return object;
	}
	
	template <meta::CoClass CoClass, meta::Interface Interface>
	auto 
	make_shared(auto&&... args)
	{
		return shared_ptr<Interface>{
			new CoClass{std::forward<decltype(args)>(args)...}
		};
	}

}
