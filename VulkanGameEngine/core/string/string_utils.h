#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "string_types.h"
#include "../typedefs.h"
//X TODO : Change this deprecated library

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <codecvt>

//X TODO : Create set of method that hashes the strings to size_t


size_t hash_string(const String& str);

_INLINE_ String string32_to_string(const String32& str)
{
	return std::wstring_convert<std::codecvt_utf8_utf16<char32_t>, char32_t>{}.to_bytes(str);
}


#endif // STRING_UTILS_H