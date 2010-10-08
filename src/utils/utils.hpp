#ifndef EE_UTILSCUTILS_H
#define EE_UTILSCUTILS_H

#include "base.hpp"

namespace EE { namespace Utils {
	/** @return True if the file exists */
	bool EE_API FileExists( const std::string& Filepath );

	/** @return The number of milliseconds since the EE++ library initialization. Note that this value wraps if the program runs for more than ~49 days. */
	Uint32 EE_API eeGetTicks();

	/** Wait a specified number of milliseconds before returning. */
	void EE_API eeSleep( const Uint32& ms );

	/** @return The application path ( the executable path ) */
	std::string EE_API AppPath();

	/** @return The files and sub directories contained by a directory */
	std::vector<std::string> EE_API FilesGetInPath( const std::string& path );

	/** @return The files and sub directories contained by a directory */
	std::vector<std::wstring> EE_API FilesGetInPath( const std::wstring& path );

	/** @return The size of a file */
	Uint32 EE_API FileSize( const std::string& Filepath );

	/** @return The System Time */
	eeDouble EE_API GetSystemTime();

	/** @return If directory exists, and is a directory */
	bool EE_API IsDirectory( const std::string& path );

	/** @return If directory exists, and is a directory */
	bool EE_API IsDirectory( const std::wstring& path );

	/** Creates a new directory */
	bool EE_API MakeDir( const std::string& path, const Uint16& mode = 0770 );

	/** @return The default windows directory */
	std::string EE_API GetWindowsPath();

	/** @return djb2 wstring hash */
	Uint32 EE_API MakeHash( const std::wstring& str );

	/** @return djb2 string hash */
	Uint32 EE_API MakeHash( const std::string& str );

	/** @return djb2 string hash */
	Uint32 EE_API MakeHash( const Int8 *str );

	/** Copy a file to memory
	* @param path The file path
	* @param data The vector to allocate the file in memory
	* @return True if returned the file to the vector.
	*/
	bool EE_API FileGet( const std::string& path, std::vector<Uint8>& data );

	/** Copy a file to location.
	* @param src Source File Path
	* @param dst Destination File Path
	* @return If success.
	*/
	bool EE_API FileCopy( const std::string& src, const std::string& dst );

	/** @return The file extension
	* @param filepath The file path or name
	* @param lowerExt Lowercase the extension obtained? ( true by default )
	*/
	std::string EE_API FileExtension( const std::string& filepath, const bool& lowerExt = true );

	/** @return The file name of a file path */
	std::string EE_API FileNameFromPath( const std::string& filepath );

	/** @return Removes the file name from a path, and return the path. */
	std::string EE_API FileRemoveFileName( const std::string& filepath );

	/** @return Removes the extension of a filepath */
	std::string EE_API FileRemoveExtension( const std::string& filepath );

	/** Write a file in binary mode and close it. */
	bool EE_API FileWrite( const std::string& filepath, const Uint8* data, const Uint32& dataSize );

	/** Write a file in binary mode and close it. */
	bool EE_API FileWrite( const std::string& filepath, const std::vector<Uint8>& data );

	/** @return The Number of CPUs of the system. */
	eeInt EE_API GetNumCPUs();

	/** @return The modification date of the file */
	Uint32 EE_API FileGetModificationDate( const std::string& Filepath );

	/** @return The File Extension of a Save Type */
	std::string EE_API SaveTypeToExtension( const Uint32& Format );

	/** If the directory path not end with a slash, it will add it. */
	void EE_API DirPathAddSlashAtEnd( std::string& path );

	/** @return The default slash path code of the current OS */
	std::string EE_API GetOSlash();

	/** Convert a size represented in bytes, to a string converted in byes/kb/mb/tb.
	* @example 10485760 -> "10.0 MB"
	*/
	std::string EE_API SizeToString( const Uint32& MemSize );

	/** Convert a size represented in bytes, to a wstring converted in byes/kb/mb/tb.
	* @example 10485760 -> "10.0 MB"
	*/
	std::wstring EE_API SizeToWString( const Uint32& MemSize );

	/** Write a bit into the Key in the position defined.
	* @param Key The Key to write
	* @param Pos The Position of the bit
	* @param BitWrite 0 for write 0, any other to write 1.
	*/
	void Write32BitKey( Uint32 * Key, Uint32 Pos, Uint32 BitWrite );

	/** Read a bit from a 32 bit key, in the position defined
	* @param Key The Key to read
	* @param Pos The Position in the key to read
	* @return True if the bit is 1
	*/
	bool Read32BitKey( Uint32 * Key, Uint32 Pos );
}

}
#endif
