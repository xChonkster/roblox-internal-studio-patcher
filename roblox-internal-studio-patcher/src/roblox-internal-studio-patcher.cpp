#include <fstream>
#include <filesystem>

#include "./registry/registry.hpp"
#include "./memory/memory.hpp"

int main(int argc, char* argv[])
{
	try
	{
		std::string roblox_studio_path{};

		if ( argc > 1 )
			roblox_studio_path = argv[1];
		else
			roblox_studio_path = registry::get_string_key( HKEY_CLASSES_ROOT, "\\roblox-studio\\DefaultIcon", "" );

		std::printf( "path: %s\n", roblox_studio_path.c_str() );

		// read file
		std::fstream studio{ roblox_studio_path, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate };
		
		if ( !studio.is_open() )
			throw std::runtime_error{ "Unable to open studio file (you dont have studio installed, or you have studio installed in program files(run as admin)" };

		std::streampos file_size = studio.tellg();
		studio.seekg( 0, std::ios::beg );

		std::uint8_t* file_buffer = static_cast<std::uint8_t*>( operator new( file_size ) );

		// we'll be writing into this
		studio.read( reinterpret_cast<char*>( file_buffer ), file_size );

		// C6 05 ? ? ? ? 00 48 8D 0D ? ? ? ? E8
		const auto scan_write_zero_to_flag = memory::aob_scan( file_buffer, file_size, "xx????xxxx????x", { 0xC6, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8D, 0x0D, 0x00, 0x00, 0x00, 0x00, 0xE8 } ); // (i kinda need a "?" symbol)

		if ( scan_write_zero_to_flag.empty() )
			throw std::runtime_error{ "Sig not matched (image is already patched, or aob is outdated)" };

		// change write 0 to write 1
		*reinterpret_cast<std::uint8_t*>( file_buffer + scan_write_zero_to_flag[8] + 6 ) = 0x01;

		// CC 88 0D ? ? ? ? C3 CC
		const auto scan_write_a1_to_flag = memory::aob_scan( file_buffer, file_size, "xxx????xx", { 0xCC, 0x88, 0x0D, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xCC } );

		// always write 1 to variable instead of a1
		const std::uint8_t patch[] =
		{
			0xCC, 0xC6, 0x05, 0x12, 0x4A, 0x9D, 0x04, 0x01, 0xC3, 0xCC
		};

		std::memcpy( file_buffer + scan_write_a1_to_flag[7], patch, sizeof( patch ) );

		// E8 ? ? ? ? 80 BF ? ? ? ? ? 74 05 E8
		const auto scan_soothsayer_check = memory::aob_scan( file_buffer, file_size, "x????xx?????xxx", { 0xE8, 0x00, 0x00, 0x00, 0x00, 0x80, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x74, 0x05, 0xE8 } );

		// nop jz
		*reinterpret_cast<std::uint16_t*>( file_buffer + scan_soothsayer_check[0] + 12 ) = 0x9090;

		// set cursor to 0
		studio.seekg( 0, std::ios::beg );

		// write buffer back into file
		studio.write( reinterpret_cast<char*>( file_buffer ), file_size );
		
		// obv
		operator delete( file_buffer );

		// done!
	}
	catch ( registry::registry_error& exception )
	{
		std::printf( "Registry error! %s (error_code: %d)\n", exception.what(), exception.get_error_code() );

		system( "PAUSE" );
	}
	catch ( std::runtime_error& exception )
	{
		std::printf( "Error! %s\n", exception.what() );

		system( "PAUSE" );
	}
}
 