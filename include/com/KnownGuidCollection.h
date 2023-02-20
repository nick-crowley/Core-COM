#pragma once
#include "library/core.COM.h"

bool
ComExport operator<(::GUID const& l, ::GUID const& r);

namespace core::com
{
	class ComExport KnownGuidCollection : private std::map<::GUID,std::string_view> {
		using base = std::map<::GUID,std::string_view>;

	public:
		using iterator = typename base::iterator;
		using const_iterator = typename base::const_iterator;
		using value_type = typename base::value_type;
		using reference = typename base::reference;
		using const_reference = typename base::const_reference;
		using difference_type = typename base::difference_type;
		using size_type = typename base::size_type;

	public:
		KnownGuidCollection();

	public:
		template <typename Self>
		auto 
		begin(this Self&& self) {
			return base::begin();
		}

		template <typename Self>
		auto 
		end(this Self&& self) {
			return base::end();
		}

		bool
		contains(::GUID const& g) const {
			return base::contains(g);
		}

		std::string_view
		operator[](::GUID const& g) const {
			return base::at(g);
		}

	public:
		void
		add(::GUID guid, std::string_view name) {
			this->emplace(guid,name);
		}
	};
	
	KnownGuidCollection
	inline ComExport KnownGuids;
}

bool
ComExport operator<(::GUID const& l, ::GUID const& r);
	