#pragma once
#include <windows.h>
#include <stdexcept>
#include <string_view>

namespace registry
{
	std::string get_string_key( HKEY registry_key, const std::string_view& subkey, const std::string_view& value );
}

