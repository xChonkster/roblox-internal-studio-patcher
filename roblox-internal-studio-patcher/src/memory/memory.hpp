#pragma once
#include <windows.h>
#include <string_view>
#include <span>
#include <vector>

namespace memory
{
	std::vector<std::uintptr_t> aob_scan( std::uint8_t* file_buffer, std::size_t file_buffer_length, const std::string_view& mask, const std::initializer_list<std::uint8_t>& aob );
}

