#pragma once

namespace core::com
{
	struct Version
	{
		unsigned Major, Minor;

		auto operator<=>(Version const&) const = default;
	};
}
