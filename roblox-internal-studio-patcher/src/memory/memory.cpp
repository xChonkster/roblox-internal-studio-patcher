#include "./memory.hpp"

namespace memory
{
	std::vector<std::uintptr_t> aob_scan( std::uint8_t* file_buffer, std::size_t file_buffer_length, const std::string_view& mask, const std::initializer_list<std::uint8_t>& aob )
	{
		std::vector<std::uintptr_t> results{};

		for (std::uint8_t* ptr = file_buffer; ptr != (file_buffer + file_buffer_length); ptr++ )
		{
			// iterate over mask so we get indices
			for ( auto index = 0u; index < mask.length(); index++ )
			{
				if ( *(ptr + index) != *( aob.begin() + index ) )
				{
					if ( mask[index] == 'x' )
						break; // if byte is not equal and we need this byte, break out of loop 
					else
						continue;
				}
				else if ( index == ( mask.length() - 1 ) ) // we reached the end of the mask, and this byte matches, we have a match!
				{
					results.push_back( static_cast<std::uintptr_t>( ptr - file_buffer ) );

					break;
				}
			}
		}

		return results;
	}
}

