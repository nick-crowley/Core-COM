#pragma once

namespace core::com
{
	struct version
	{
		unsigned Major, Minor;

		auto operator<=>(version const&) const = default;
	};
}
