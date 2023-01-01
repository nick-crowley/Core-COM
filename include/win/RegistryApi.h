#pragma once
#include "comFramework.h"
#include "win/SmartHandle.h"
#include "win/RegistryValue.h"

namespace win
{
	class RegistryApi
	{
	public:
		enum Lifetime { Volatile, NonVolatile };
		enum View { Wow32, Wow64 };

		virtual ~RegistryApi() = default;

		SharedRegistryKey
		virtual create_key(SharedRegistryKey root, std::wstring_view path, ::REGSAM rights, Lifetime l = NonVolatile) const;

		SharedRegistryKey
		virtual open_key(SharedRegistryKey root, std::wstring_view path, ::REGSAM rights) const;
		
		RegistryValue
		virtual get_value(SharedRegistryKey root, std::wstring_view path, std::wstring_view name) const;
		
		void
		virtual set_value(SharedRegistryKey root, std::wstring_view path, std::wstring_view name, RegistryValue value) const;
		
		void
		virtual remove_key(SharedRegistryKey root, std::wstring_view path) const;
		
		void
		virtual remove_value(SharedRegistryKey root, std::wstring_view path, std::wstring_view name) const;
	};
}
