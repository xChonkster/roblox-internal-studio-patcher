#include <iostream>

#include "./memory/memory.hpp"

int main()
{
	std::uint8_t memory[] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00, 0x01, 0x00
	};

	const auto scan = memory::aob_scan( memory, sizeof(memory), "xx", { 0x00, 0x01 });

	for ( const auto& result : scan )
		std::printf( "scan result: %llu\n", result );
}

