#pragma once
#include "library/core.COM.h"
#include "com/Function.h"

namespace core::com::detail
{
	template <typename ValueType, meta::Interface CoClass, meta::Invocable_r<ValueType,CoClass&> Getter, std::invocable<CoClass&,ValueType> Setter>
	class MutablePropertyProxy
	{
		using type = MutablePropertyProxy<ValueType,CoClass,Getter,Setter>;

	private:
		CoClass& m_object;
		Getter   m_get;
		Setter   m_set;

	public:
		constexpr
		explicit
		MutablePropertyProxy(CoClass& obj, Getter get, Setter set)
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
	
	template <typename ValueType, typename Getter, typename Setter>
	class MutablePropertyFunctor
	{
		using type = MutablePropertyFunctor<ValueType,Getter,Setter>;

	private:
		Getter   m_get;
		Setter   m_set;

	public:
		constexpr
		MutablePropertyFunctor(Getter get, Setter set)
		  : m_get{get}, m_set{set}
		{}
		
		satisfies(MutablePropertyFunctor,
			constexpr IsCopyable,
			constexpr IsMovable,
			NotEqualityComparable,
			NotSortable
		);

	public:
		template <meta::Interface CoClass>
		auto constexpr
		operator()(CoClass& obj) const {
			return MutablePropertyProxy<ValueType,CoClass,Getter,Setter>{obj, this->m_get, this->m_set};
		}
	};

	template <typename ValueType, meta::Interface CoClass, meta::Invocable_r<ValueType,CoClass&> Getter>
	class ReadOnlyPropertyProxy
	{
		using type = ReadOnlyPropertyProxy<ValueType,CoClass,Getter>;

	private:
		CoClass& m_object;
		Getter   m_get;

	public:
		constexpr
		explicit
		ReadOnlyPropertyProxy(CoClass& obj, Getter get)
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
	
	template <typename ValueType, typename Getter>
	class ReadOnlyPropertyFunctor
	{
		using type = ReadOnlyPropertyFunctor<ValueType,Getter>;

	private:
		Getter   m_get;

	public:
		constexpr
		explicit
		ReadOnlyPropertyFunctor(Getter get)
		  : m_get{get}
		{}
		
		satisfies(ReadOnlyPropertyFunctor,
			constexpr IsCopyable,
			constexpr IsMovable,
			NotEqualityComparable,
			NotSortable
		);

	public:
		template <meta::Interface CoClass>
		auto constexpr
		operator()(CoClass& obj) const {
			return ReadOnlyPropertyProxy<ValueType,CoClass,Getter>{obj, this->m_get};
		}
	};
}

namespace core::com
{
	template <typename ValueType, meta::Interface Interface>
	auto constexpr
	property(::HRESULT (__stdcall Interface::*get)(ValueType*), ::HRESULT (__stdcall Interface::*set)(ValueType))
		-> detail::MutablePropertyFunctor<ValueType,decltype(com::method<1>(get)),decltype(com::method(set))>
	{
		return { com::method<1>(get), com::method(set) };
	}

	template <typename ValueType, meta::Interface Interface>
	auto constexpr
	property(::HRESULT (__stdcall Interface::*get)(ValueType*))
	{
		return detail::ReadOnlyPropertyFunctor<ValueType,decltype(com::method<1>(get))>{ com::method<1>(get) };
	}
}
