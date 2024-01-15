#include "./registry.hpp"

namespace registry
{
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
			throw registry_error{ "Unable to read registry string key (try dragging RobloxStudioBeta.exe onto this file (this error shouldnt happen)", status };

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
			throw registry_error{ "Unable to read registry string key (try dragging RobloxStudioBeta.exe onto this file (this error shouldnt happen)", status };
		
		// null termination potentially
		data.resize( string_lenth_in_chars );

		return data;
	} // should entirely be wstring...
}

