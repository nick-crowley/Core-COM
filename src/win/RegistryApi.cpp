#include "win/RegistryApi.h"
#include "win/Function.h"
#include "win/ApiHelpers.h"
#include "win/DWord.h"
#include "win/LResult.h"
#include "win/ConstRegistryStringIterator.h"

namespace
{
	auto constexpr regCreateKeyEx = win::function<2>(::RegCreateKeyExW);	
	auto constexpr regOpenKeyEx = win::function<1>(::RegOpenKeyExW);
	auto constexpr regDeleteKeyEx = win::function(::RegDeleteKeyExW);
	auto constexpr regDeleteKeyValue = win::function(::RegDeleteKeyValueW);
	auto constexpr regQueryValueEx = win::function<3>(::RegQueryValueExW);
	auto constexpr regSetKeyValue = win::function(::RegSetKeyValueW);
}

win::SharedRegistryKey
win::RegistryApi::create_key(SharedRegistryKey root, std::wstring_view path, ::REGSAM rights, Lifetime l) const
{
	auto const flags = l == NonVolatile ? REG_OPTION_NON_VOLATILE : REG_OPTION_VOLATILE;
	auto [key, disposition] = regCreateKeyEx(*root, path.data(), Reserved<DWORD>, nullptr, flags, rights, Unsecured);
	return SharedRegistryKey{key};
}

win::SharedRegistryKey
win::RegistryApi::open_key(SharedRegistryKey root, std::wstring_view path, ::REGSAM rights) const
{
	constexpr DWORD NotASymLink = NULL;
	return SharedRegistryKey{regOpenKeyEx(*root, path.data(), NotASymLink, rights)};
}


win::RegistryValue
win::RegistryApi::get_value(SharedRegistryKey root, std::wstring_view path, std::wstring_view name) const
{
	auto [type, data, size] = regQueryValueEx(*root, path.data(), Reserved<DWORD*>);
	auto value = std::make_unique<std::byte[]>(size);
	LResult result = ::RegQueryValueExW(*root, path.data(), Reserved<DWORD*>, nullptr, (BYTE*)value.get(), &size);
	
	switch (type)
	{
	default:
		win::throw_exception(ERROR_UNSUPPORTED_TYPE);

	case REG_BINARY:
		return std::vector<std::byte>{&value[0], &value[size]};

	case REG_DWORD:
		return *boost::reinterpret_pointer_cast<std::uint32_t>(std::move(value));

	case REG_QWORD:
		return *boost::reinterpret_pointer_cast<std::uint64_t>(std::move(value));

	case REG_SZ:
	case REG_MULTI_SZ:
		break;
	}

	auto const str = boost::reinterpret_pointer_cast<wchar_t[]>(std::move(value));
	if (type == REG_SZ)
		return std::wstring{&str[0], &str[size/sizeof(wchar_t)]};
	
	return std::vector<std::wstring>{ 
		ConstRegistryStringIterator{{&str[0], &str[size/sizeof(wchar_t)]}}, 
		ConstRegistryStringIterator{} 
	};
}
	
void
win::RegistryApi::set_value(SharedRegistryKey root, std::wstring_view path, std::wstring_view name, RegistryValue value) const
{
	if (auto* bytes = std::get_if<std::vector<std::byte>>(&value))
	{
		regSetKeyValue(*root, path.data(), name.data(), REG_BINARY, bytes->data(), DWord{bytes->size()});
	}
	else if (auto* int32 = std::get_if<std::uint32_t>(&value))
	{
		regSetKeyValue(*root, path.data(), name.data(), REG_DWORD, int32, 4);
	}
	else if (auto* int64 = std::get_if<std::uint64_t>(&value))
	{
		regSetKeyValue(*root, path.data(), name.data(), REG_QWORD, int64, 8);
	}
	else if (auto* str = std::get_if<std::wstring>(&value))
	{
		regSetKeyValue(*root, path.data(), name.data(), REG_SZ, str->data(), DWord{str->size()*sizeof(wchar_t)}); 
	}
	else if (auto* str = std::get_if<std::wstring_view>(&value))
	{
		regSetKeyValue(*root, path.data(), name.data(), REG_SZ, str->data(), DWord{str->size()*sizeof(wchar_t)}); 
	}
	else if (auto* multi = std::get_if<std::vector<std::wstring>>(&value))
	{
		auto constexpr static concat = [](std::wstring const& total, std::wstring const& e){ return total + e + L'\0'; };

		std::wstring const flat = std::accumulate(multi->begin(), multi->end(), std::wstring{}, concat) + L'\0';
		regSetKeyValue(*root, path.data(), name.data(), REG_MULTI_SZ, flat.c_str(), DWord{flat.size()*sizeof(wchar_t)}); 
	}
}

void
win::RegistryApi::remove_key(SharedRegistryKey root, std::wstring_view name) const
{
	constexpr DWORD NotUsingWowFlag = NULL;
	regDeleteKeyEx(*root, name.data(), NotUsingWowFlag, Reserved<DWORD>);
}
		
void
win::RegistryApi::remove_value(SharedRegistryKey root, std::wstring_view path, std::wstring_view name) const
{
	regDeleteKeyValue(*root, path.data(), name.data());
}
