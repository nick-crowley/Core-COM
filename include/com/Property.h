#pragma once
#include "library/core.COM.h"
#include "com/Function.h"

namespace core::com::detail
{
	template <typename ValueType, meta::Interface Interface>
	using get_method_t = method_t<1,Interface,ValueType*>;

	template <typename ValueType, meta::Interface Interface>
	using put_method_t = method_t<0,Interface,ValueType>;

	template <typename ValueType, meta::Interface Interface>
	class MutablePropertyProxy
	{
		using type = MutablePropertyProxy<ValueType,Interface>;
		using GetMethod = get_method_t<ValueType,Interface>;
		using SetMethod = put_method_t<ValueType,Interface>;

	private:
		Interface& m_object;
		GetMethod  m_get;
		SetMethod  m_set;

	public:
		constexpr
		explicit
		MutablePropertyProxy(Interface& obj, GetMethod get, SetMethod set)
		  : m_object{obj}, m_get{get}, m_set{set}
		{}

		satisfies(MutablePropertyProxy,
			NotCopyable,
			NotMovable,
			NotEqualityComparable,
			NotSortable
		);

	public:
		type&
		operator=(ValueType v) {
			this->m_set(this->m_object, v);
			return *this;
		}

		implicit operator
		ValueType() const {
			return this->m_get(this->m_object);
		}
	};
	
	template <typename ValueType, meta::Interface Interface>
	class MutablePropertyFunctor
	{
		using type = MutablePropertyFunctor<ValueType,Interface>;
		using GetMethod = get_method_t<ValueType,Interface>;
		using SetMethod = put_method_t<ValueType,Interface>;

	private:
		GetMethod  m_get;
		SetMethod  m_set;

	public:
		constexpr
		MutablePropertyFunctor(GetMethod get, SetMethod set)
		  : m_get{get}, m_set{set}
		{}
		
		satisfies(MutablePropertyFunctor,
			constexpr IsCopyable,
			constexpr IsMovable,
			NotEqualityComparable,
			NotSortable
		);

	public:
		auto constexpr
		operator()(Interface& obj) const {
			return MutablePropertyProxy<ValueType,Interface>{obj, this->m_get, this->m_set};
		}
	};

	template <typename ValueType, meta::Interface Interface>
	class ReadOnlyPropertyProxy
	{
		using type = ReadOnlyPropertyProxy<ValueType,Interface>;
		using GetMethod = get_method_t<ValueType,Interface>;

	private:
		Interface& m_object;
		GetMethod  m_get;

	public:
		constexpr
		explicit
		ReadOnlyPropertyProxy(Interface& obj, GetMethod get)
		  : m_object{obj}, m_get{get}
		{}

		satisfies(ReadOnlyPropertyProxy,
			NotCopyable,
			NotMovable,
			NotEqualityComparable,
			NotSortable
		);

	public:
		type&
		operator=(ValueType) = delete;

		implicit operator
		ValueType() const {
			return this->m_get(this->m_object);
		}
	};
	
	template <typename ValueType, meta::Interface Interface>
	class ReadOnlyPropertyFunctor
	{
		using type = ReadOnlyPropertyFunctor<ValueType,Interface>;
		using GetMethod = get_method_t<ValueType,Interface>;

	private:
		GetMethod m_get;

	public:
		constexpr
		explicit
		ReadOnlyPropertyFunctor(GetMethod get)
		  : m_get{get}
		{}
		
		satisfies(ReadOnlyPropertyFunctor,
			constexpr IsCopyable,
			constexpr IsMovable,
			NotEqualityComparable,
			NotSortable
		);

	public:
		auto constexpr
		operator()(Interface& obj) const {
			return ReadOnlyPropertyProxy<ValueType,Interface>{obj, this->m_get};
		}
	};
}

namespace core::com
{
	template <typename ValueType, meta::Interface Interface>
	using const_property_t = detail::ReadOnlyPropertyFunctor<ValueType,Interface>;

	template <typename ValueType, meta::Interface Interface>
	using mutable_property_t = detail::MutablePropertyFunctor<ValueType,Interface>; 

	template <typename ValueType, meta::Interface Interface>
	mutable_property_t<ValueType,Interface> constexpr
	property(method_pointer_t<Interface,ValueType*> get, method_pointer_t<Interface,ValueType> set)
	{
		return { com::method<1>(get), com::method(set) };
	}
	
	template <typename ValueType, meta::Interface Interface>
	const_property_t<ValueType,Interface> constexpr
	property(method_pointer_t<Interface,ValueType*> get)
	{
		return const_property_t<ValueType,Interface>{ com::method<1>(get) };
	}
}
