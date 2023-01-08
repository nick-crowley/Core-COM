#include "win/RegistryKey.h"
using win::RegistryKey;

RegistryKey::RegistryKey(meta::open_existing_t, SharedRegistryKey handle, REGSAM rights, 
	std::shared_ptr<RegistryApi> api)
	: m_api{ThrowIfEmpty(api)}, m_key{handle}, m_rights{rights}
{
}

RegistryKey::RegistryKey(meta::open_existing_t, RegistryKey const& key, REGSAM rights, std::shared_ptr<RegistryApi> api)
	: RegistryKey{meta::open_existing, key.m_key, rights, api}
{
}

RegistryKey::RegistryKey(meta::open_existing_t, SharedRegistryKey parent, std::wstring_view child, REGSAM rights, 
	std::shared_ptr<RegistryApi> api)
	: m_api{ThrowIfEmpty(api)}, m_key{api->open_key(parent, child, rights)}, m_rights{rights}
{
}

RegistryKey::RegistryKey(meta::open_existing_t, RegistryKey const& key, std::wstring_view child, REGSAM rights, 
	std::shared_ptr<RegistryApi> api)
	: RegistryKey{meta::open_existing, key.m_key, child, rights, api}
{
}

RegistryKey::RegistryKey(meta::create_new_t, SharedRegistryKey parent, std::wstring_view child, REGSAM rights, 
	std::shared_ptr<RegistryApi> api)
	: m_api{ThrowIfEmpty(api)}, m_key{api->create_key(parent, child, rights)}, m_rights{rights}
{
}

RegistryKey::RegistryKey(meta::create_new_t, RegistryKey const& parent, std::wstring_view child, REGSAM rights, 
	std::shared_ptr<RegistryApi> api)
	: RegistryKey{meta::create_new, parent.m_key, child, rights, api}
{
}

RegistryKey 
RegistryKey::createSubKey(std::wstring_view name) const
{
	return RegistryKey{
		meta::open_existing, 
		this->m_api->create_key(this->m_key, ThrowIfEmpty(name), this->m_rights), 
		this->m_rights, 
		this->m_api 
	};
}

void 
RegistryKey::deleteSubKey(std::wstring_view name) const
{
	this->m_api->remove_key(this->m_key, ThrowIfEmpty(name));
}

win::RegistryValue 
RegistryKey::getValue() const
{
	return m_api->get_value(this->m_key, {}, {});
}

win::RegistryValue 
RegistryKey::getValue(std::wstring_view name) const
{
	return m_api->get_value(this->m_key, {}, ThrowIfEmpty(name));
}

void 
RegistryKey::setValue(RegistryValue&& v) const
{
	return m_api->set_value(this->m_key, {}, {}, v);
}

void 
RegistryKey::setValue(std::wstring_view name, RegistryValue&& v) const
{
	return m_api->set_value(this->m_key, {}, ThrowIfEmpty(name), v);
}
