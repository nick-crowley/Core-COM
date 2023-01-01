#pragma once
#include "comFramework.h"

namespace win
{
	class ConstRegistryStringIterator 
		: public boost::iterator_facade<ConstRegistryStringIterator, std::wstring_view const, std::forward_iterator_tag>
	{
		friend class boost::iterator_core_access;
		using type = ConstRegistryStringIterator;

		std::wstring_view const
		inline static npos;

	private:
		std::wstring_view  m_current;
		std::wstring_view  m_source;

	public:
		ConstRegistryStringIterator()
		  : m_source{npos}
		{
		}

		ConstRegistryStringIterator(std::wstring_view src)
		  : m_source{src}
		{
		}

	private:
		void
		increment() 
		{ 
			if (this->m_current.front() == L'\0')
				this->m_current = npos;
			else
				this->m_current.remove_prefix(this->m_current.find(L'\0')+1);
		}

		bool 
		equal(type const& other) const
		{
			return this->m_current == other.m_current;
		}

		std::wstring_view const& 
		dereference() const 
		{ 
			return this->m_current; 
		}
	};
}
