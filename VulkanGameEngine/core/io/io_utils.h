#ifndef IO_UTILS_H
#define IO_UTILS_H

#include "../string/string_types.h"
#include "../typedefs.h"
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

_INLINE_ bool is_path_exist(const String& path)
{
	return boost::filesystem::exists(path);
}

_INLINE_ bool is_path_exist(const String16& path)
{
	// Convert u16string to wstring
	std::wstring wpath = boost::locale::conv::utf_to_utf<wchar_t>(path);

	boost::filesystem::path fs_path(wpath);
	return boost::filesystem::exists(fs_path);
}
_INLINE_ bool is_path_exist(const std::wstring& path)
{
	boost::filesystem::path fs_path(path);
	return boost::filesystem::exists(fs_path);
}

_INLINE_ void create_path(const String16& path)
{
	// Convert u16string to wstring
	std::wstring wpath = boost::locale::conv::utf_to_utf<wchar_t>(path);

	boost::filesystem::create_directory(wpath);
}
#endif
