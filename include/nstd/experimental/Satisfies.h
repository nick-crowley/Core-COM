#pragma once
#ifndef ComFramework_h_included
#	error Including this header directly may cause a circular dependency; include <comFramework.h> directly
#endif
#include "../src/libBoost.h"

namespace nstd
{	
    
// Defaulted named requirements
#define IsDefaultConstructible(className)   className() = default
#define IsDestructible(className)           ~className() = default
#define IsCopyConstructible(className)      className(className const&) = default
#define IsCopyAssignable(className)         className& operator=(className const&) = default
#define IsMoveConstructible(className)      className(className&&) = default
#define IsMoveAssignable(className)         className& operator=(className&&) = default
#define IsEqualityComparable(className)     bool operator==(className const&) const = default
#define IsSortable(className)               auto operator<=>(className const&) const = default
#define IsCopyable(className)				IsCopyConstructible(className); IsCopyAssignable(className)
#define IsMovable(className)				IsMoveConstructible(className); IsMoveAssignable(className)

// Deleted named requirements
#define NotDefaultConstructible(className)  className() = delete
#define NotDestructible(className)          ~className() = delete
#define NotCopyConstructible(className)     className(className const&) = delete
#define NotCopyAssignable(className)        className& operator=(className const&) = delete
#define NotMoveConstructible(className)     className(className&&) = delete
#define NotMoveAssignable(className)        className& operator=(className&&) = delete
#define NotEqualityComparable(className)    bool operator==(className const&) const = delete
#define NotSortable(className)              auto operator<=>(className const&) const = delete
#define NotCopyable(className)				NotCopyConstructible(className); NotCopyAssignable(className)
#define NotMovable(className)				NotMoveConstructible(className); NotMoveAssignable(className)

// noexcept support
#define IsDefaultConstructible_noexcept(className)  className() noexcept = default
#define IsDestructible_noexcept(className)          ~className() noexcept = default
#define IsCopyConstructible_noexcept(className)     className(className const&) noexcept = default
#define IsCopyAssignable_noexcept(className)        className& operator=(className const&) noexcept = default
#define IsMoveConstructible_noexcept(className)     className(className&&) noexcept = default
#define IsMoveAssignable_noexcept(className)        className& operator=(className&&) noexcept = default
#define IsEqualityComparable_noexcept(className)    bool operator==(className const&) const noexcept = default
#define IsSortable_noexcept(className)              auto operator<=>(className const&) const noexcept = default
#define IsCopyable_noexcept(className)				IsCopyConstructible_noexcept(className); IsCopyAssignable_noexcept(className)
#define IsMovable_noexcept(className)				IsMoveConstructible_noexcept(className); IsMoveAssignable_noexcept(className)

// constexpr support
#define constexpr_IsCopyable(className)				constexpr IsCopyConstructible(className); constexpr IsCopyAssignable(className)
#define constexpr_IsMovable(className)				constexpr IsMoveConstructible(className); constexpr IsMoveAssignable(className)
#define constexpr_NotCopyable(className)			constexpr NotCopyConstructible(className); constexpr NotCopyAssignable(className)
#define constexpr_NotMovable(className)				constexpr NotMoveConstructible(className); constexpr NotMoveAssignable(className)

// constexpr/noexcept support
#define constexpr_IsCopyable_noexcept(className)	constexpr IsCopyConstructible_noexcept(className); constexpr IsCopyAssignable_noexcept(className)
#define constexpr_IsMovable_noexcept(className)		constexpr IsMoveConstructible_noexcept(className); constexpr IsMoveAssignable_noexcept(className)

#define _makeNamedRequirement(s,className,namedRequirement)      \
	namedRequirement(className);

#define _expandNamedRequirements(className,...)      \
	BOOST_PP_SEQ_FOR_EACH(_makeNamedRequirement, className, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

//! @brief	Expands named-requirements into defaulted/deleted method declarations
#define satisfies(className, ...)		\
	__VA_OPT__(_expandNamedRequirements(className,__VA_ARGS__))


} // namespace nstd
