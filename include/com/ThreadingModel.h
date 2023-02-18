#pragma once

namespace core::com
{
	enum class ThreadingModel
	{
		Isolated,   //!< Apartment
		Shared,     //!< Free
		Any         //!< Both
	};
}