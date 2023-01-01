#pragma once

namespace com
{
	struct version
	{
		unsigned Major, Minor;

		auto operator<=>(version const&) const = default;
	};
}
