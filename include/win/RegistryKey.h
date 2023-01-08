#pragma once
#include "comFramework.h"
#include "win/SmartHandle.h"
#include "win/RegistryValue.h"
#include "win/RegistryApi.h"

struct open_existing_t
{ 
	satisfies(open_existing_t,
		constexpr IsDefaultConstructible_noexcept
	);
}
constexpr 
inline open_existing;

struct create_new_t
{ 
	satisfies(create_new_t,
		constexpr IsDefaultConstructible_noexcept
	);
}
constexpr 
inline create_new;

namespace win
{
	SharedRegistryKey const 
	inline ClassesRoot {HKEY_CLASSES_ROOT};

	SharedRegistryKey const 
	inline CurrentUser {HKEY_CURRENT_USER};

	SharedRegistryKey const 
	inline LocalMachine {HKEY_LOCAL_MACHINE};

	class RegistryKey
	{
		std::shared_ptr<RegistryApi>  m_api;
		SharedRegistryKey m_key;
		REGSAM m_rights;

	public:
		//! @brief	Open existing key
		RegistryKey(open_existing_t, SharedRegistryKey handle, REGSAM rights, 
			std::shared_ptr<RegistryApi> api = std::make_shared<RegistryApi>());

		//! @brief	Open existing key
		RegistryKey(open_existing_t, RegistryKey const& key, REGSAM rights, 
			std::shared_ptr<RegistryApi> api = std::make_shared<RegistryApi>());

		//! @brief	Open child of existing key
		RegistryKey(open_existing_t, SharedRegistryKey parent, std::wstring_view child, REGSAM rights, 
			std::shared_ptr<RegistryApi> api = std::make_shared<RegistryApi>());

		//! @brief	Open child of existing key
		RegistryKey(open_existing_t, RegistryKey const& parent, std::wstring_view child, REGSAM rights, 
			std::shared_ptr<RegistryApi> api = std::make_shared<RegistryApi>());

		//! @brief	Create child of existing key
		RegistryKey(create_new_t, SharedRegistryKey parent, std::wstring_view child, REGSAM rights, 
			std::shared_ptr<RegistryApi> api = std::make_shared<RegistryApi>());
		
		//! @brief	Create child of existing key
		RegistryKey(create_new_t, RegistryKey const& parent, std::wstring_view child, REGSAM rights, 
			std::shared_ptr<RegistryApi> api = std::make_shared<RegistryApi>());

		RegistryKey 
		createSubKey(std::wstring_view name) const;

		void 
		deleteSubKey(std::wstring_view name) const;
		
		RegistryValue 
		getValue() const;

		RegistryValue 
		getValue(std::wstring_view name) const;

		void 
		setValue(RegistryValue&& v) const;
		
		void 
		setValue(std::wstring_view name, RegistryValue&& v) const;
	};
}
