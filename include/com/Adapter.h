#pragma once
#include "library/core.COM.h"
#include "com/Function.h"
#include "com/Property.h"
#include "com/SharedPtr.h"

namespace core::com
{
	template <meta::Interface Interface>
	class adapter {
		using type = adapter<Interface>;

	private:
		Interface& m_object;
	
	protected:
		using interface_type = Interface;

		template <typename ValueType>
		using const_property_t = detail::ReadOnlyPropertyProxy<ValueType,interface_type>;

		template <typename ValueType>
		using mutable_property_t = detail::MutablePropertyProxy<ValueType,interface_type>; 

		template <unsigned NumResult, typename... Parameters>
		using method_t = decltype(std::bind_front(
			std::declval<com::method_t<NumResult,interface_type,Parameters...>>(),
			std::declval<std::reference_wrapper<Interface>>()
		));

	protected:
		adapter(interface_type& obj) : m_object{obj}
		{}

	protected:
		template <unsigned NumReturnParameters = 0, typename... Parameters>
		auto	
		method(method_pointer_t<Interface,Parameters...> method) const {
			return std::bind_front(com::method<NumReturnParameters>(method), std::ref( const_cast<Interface&>(this->m_object) ));
		}

		template <typename ValueType>
		const_property_t<ValueType>
		property(method_pointer_t<Interface,ValueType*> get) const {
			return com::property(get)(this->m_object);
		}
		
		template <typename ValueType>
		mutable_property_t<ValueType> 
		property(method_pointer_t<Interface,ValueType*> get, method_pointer_t<Interface,ValueType> set) const {
			return com::property(get, set)(this->m_object);
		}
	};

}