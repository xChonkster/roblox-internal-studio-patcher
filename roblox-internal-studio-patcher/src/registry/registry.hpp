#pragma once
#include <windows.h>
#include <stdexcept>
#include <string_view>

namespace registry
{
	class registry_error
		: public std::runtime_error
	{
	public:
		registry_error(const std::string_view& message, LONG error_code)
			: std::runtime_error{ message.data()}, m_error_code(error_code)
		{
		}

	public:
		constexpr LONG get_error_code()
		{
			return m_error_code;
		}

	private:
		LONG m_error_code;
	};

	std::string get_string_key( HKEY registry_key, const std::string_view& subkey, const std::string_view& value );
}

