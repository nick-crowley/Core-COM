#pragma once
#ifndef ComFramework_h_included
#	error Including this header directly may cause a circular dependency; include <comFramework.h> directly
#endif
#include "nstd/experimental/Satisfies.h"

namespace meta
{
	struct adopt_t
	{ 
		satisfies(adopt_t,
			constexpr IsDefaultConstructible_noexcept
		);
	}
	constexpr 
	inline adopt;
	

	struct create_new_t
	{ 
		satisfies(create_new_t,
			constexpr IsDefaultConstructible_noexcept
		);
	}
	constexpr 
	inline create_new;


	struct hidden_t
	{
		satisfies(hidden_t,
			constexpr IsDefaultConstructible_noexcept
		);
	}
	constexpr  
	inline hidden;


	struct open_existing_t
	{ 
		satisfies(open_existing_t,
			constexpr IsDefaultConstructible_noexcept
		);
	}
	constexpr 
	inline open_existing;


    struct use_default_t 
    {
        satisfies(use_default_t,
		    constexpr IsDefaultConstructible_noexcept
	    );
    } 
    constexpr inline 
    use_default;
}

//using adopt_t = meta::adopt_t;
//using create_new_t = meta::create_new_t;
//using hidden_t = meta::hidden_t;
//using open_existing_t = meta::open_existing_t;
//using use_default_t = meta::use_default_t;

auto constexpr 
inline adopt = meta::adopt;

auto constexpr 
inline create_new = meta::create_new;

auto constexpr 
inline hidden = meta::hidden;

auto constexpr 
inline open_existing = meta::open_existing;

auto constexpr 
inline use_default = meta::use_default;
