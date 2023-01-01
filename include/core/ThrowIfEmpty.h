#pragma once
//#include "comFramework.h"       // Warning: To avoid circular dependency, include <comFramework.h> directly
#include "meta/TypeTraits.h"

template <typename T> 
	requires std::is_same_v<std::remove_reference_t<T>,bool> 
	      || std::is_arithmetic_v<std::remove_reference_t<T>>
	      || std::is_pointer_v<std::remove_reference_t<T>>
bool 
empty(T&& value)
{
	return value == static_cast<std::remove_reference_t<T>>(0);
}

namespace std 
{
	template <typename T>
	bool
	empty(std::shared_ptr<T> const& value)
	{
		return !static_cast<bool>(value);
	}

	template <typename T>
	bool
	empty(std::unique_ptr<T> const& value)
	{
		return !static_cast<bool>(value);
	}

	template <typename T>
	bool
	empty(std::weak_ptr<T> const& value)
	{
		return !value.expired();
	}
}

template <typename T> 
std::add_rvalue_reference_t<T>
inline ThrowIfEmptyImpl(T&& value, char const* argName, std::source_location loc = std::source_location::current())
{
	using std::empty;
	using ::empty;

	/*BUG: static_assert(meta::EmptyCompatible<int*&>);
	BUG: static_assert(meta::EmptyCompatible<wchar_t **&>);
	BUG: static_assert(meta::EmptyCompatible<unsigned long *&>);
	BUG: static_assert(meta::EmptyCompatible<std::shared_ptr<void>&>);

	static_assert(meta::EmptyCompatible<T>);*/

	if (empty(value))
		throw std::invalid_argument{std::format("{}(..) Empty '{}' argument", loc.function_name(), argName)};

	return std::forward<T>(value);
}

#define ThrowIfEmpty(arg)  ThrowIfEmptyImpl(arg, #arg)
