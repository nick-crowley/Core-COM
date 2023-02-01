#pragma once
#include "library/core.COM.h"

namespace core::com
{
	struct Version
	{
		unsigned Major = 1, Minor = 0;

		constexpr
		Version(unsigned major, unsigned minor)
		  : Major{major}, Minor{minor}
		{}

		satisfies(Version,
			constexpr IsRegular noexcept,
			constexpr IsSortable noexcept
		);
	};
}
