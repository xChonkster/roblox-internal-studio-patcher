#include "./registry.hpp"

namespace registry
{
	NTSTATUS open_key( HKEY root_key, const std::wstring_view& key_path, std::uint32_t permission, HKEY& registry_key )
	{	
		return RegOpenKeyExW( root_key, key_path.data(), 0, permission, &registry_key );
	}

	std::string get_string_key( HKEY registry_key, const std::string_view& subkey, const std::string_view& value )
	{
		DWORD data_size{};
		LONG status = ::RegGetValue(
			registry_key,
			subkey.data(),
			value.data(),
			RRF_RT_REG_SZ,
			nullptr,
			nullptr,
			&data_size
		);
		
		if ( status != ERROR_SUCCESS )
			throw std::runtime_error{ "Unable to read registry string key" };

		DWORD string_lenth_in_chars = data_size / sizeof( char );

		std::string data;
		data.resize( string_lenth_in_chars );

		status = ::RegGetValue(
			registry_key,
			subkey.data(),
			value.data(),
			RRF_RT_REG_SZ,
			nullptr,
			&data[0],
			&data_size
		);

		if ( status != ERROR_SUCCESS )
			throw std::runtime_error{ "Unable to read registry string key" };
		
		// null termination potentially
		data.resize( string_lenth_in_chars );

		return data;
	} // should entirely be wstring...
}

