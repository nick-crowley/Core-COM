#pragma once
#include "library/core.COM.h"
#include "com/Function.h"
#include "com/Property.h"

namespace core::com
{
	template <typename Interface>
	class adapter {
		Interface& m_object;
	
	protected:
		using interface_type = Interface;

	protected:
		adapter(interface_type& obj) : m_object{obj}
		{}

	protected:
		template <unsigned NumReturnParameters = 0, typename... Parameters>
		auto method(::HRESULT (__stdcall Interface::*method)(Parameters...), auto... args) {
			return com::method<NumReturnParameters>(method)(this->m_object, args...);
		}

		template <typename ValueType>
		auto property(::HRESULT (__stdcall Interface::*get)(ValueType*)) {
			return com::property(get)(this->m_object);
		}
		
		template <typename ValueType>
		auto property(::HRESULT (__stdcall Interface::*get)(ValueType*), 
		              ::HRESULT (__stdcall Interface::*set)(ValueType)) {
			return com::property(get, set)(this->m_object);
		}
	};

}