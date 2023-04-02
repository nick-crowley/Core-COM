#pragma once
#include "library/core.COM.h"
#pragma warning (push)
#pragma warning (disable: 4251)

bool
ComExport operator<(::GUID const& l, ::GUID const& r);

namespace std 
{
    template <>
	struct hash<::GUID> 
	{
		size_t
		operator()(::GUID const& g) const noexcept 
		{
			size_t seed = 0;
			// Adapted from boost::uuid
			for (byte const b : as_bytes(span(&g,1))) 
				seed ^= static_cast<size_t>(b) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}

namespace core::com
{
	class ComExport KnownGuidCollection : private std::unordered_map<::GUID,std::string_view> {
		using base = std::unordered_map<::GUID,std::string_view>;

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
	extern ComExport KnownGuids;
}

#pragma warning (pop)