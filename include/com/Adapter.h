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

	protected:
		using interface_type = Interface;

		template <typename ValueType>
		using const_property_t = detail::ReadOnlyPropertyProxy<ValueType,interface_type>;

		template <typename ValueType>
		using mutable_property_t = detail::MutablePropertyProxy<ValueType,interface_type>; 

	private:
		com::shared_ptr<Interface> m_object;
	
	protected:
		adapter(com::shared_ptr<Interface> ptr) : m_object{std::move(ptr)}
		{}

	public:
		com::shared_ptr<Interface>
		ptr() const {
			return this->m_object;
		}

		implicit operator
		interface_type*() const {
			return this->m_object;
		}

	protected:
		template <unsigned NumReturnParams = 0, typename... Parameters>
		auto
		method(method_pointer_t<Interface,Parameters...> method) const {
			return std::bind_front(com::method<NumReturnParams>(method), std::ref(const_cast<Interface&>(*this->m_object)));
		}

		// FIXME: This cannot preceed the above declaration because of its auto return-type
		template <unsigned NumReturnParams, auto Method>
		using method_t = decltype(std::declval<type const>().method<NumReturnParams>(Method));
		
		template <typename ValueType>
		const_property_t<ValueType>
		property(method_pointer_t<Interface,ValueType*> get) const {
			return com::property(get)(*this->m_object);
		}
		
		template <typename ValueType>
		mutable_property_t<ValueType> 
		property(method_pointer_t<Interface,ValueType*> get, method_pointer_t<Interface,ValueType> set) const {
			return com::property(get, set)(*this->m_object);
		}
	};

}