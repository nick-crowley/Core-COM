#pragma once

namespace core::com
{
	enum ThreadingModel
	{
		Isolated,   //!< Apartment
		Shared,     //!< Free
		Any         //!< Both
	};
}