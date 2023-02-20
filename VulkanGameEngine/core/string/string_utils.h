#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "string_types.h"
#include "../typedefs.h"
#include <stdexcept>

//X TODO : Create set of method that hashes the strings to size_t


size_t hash_string(const String& str);

_INLINE_ String16 string32_to_string16(const String32& str)
{
    String16 result;
    for (auto c : str) {
        if (c <= 0xFFFF) {
            result += static_cast<char16_t>(c);
        }
        else {
            c -= 0x10000;
            result += static_cast<char16_t>(0xD800 | (c >> 10));
            result += static_cast<char16_t>(0xDC00 | (c & 0x3FF));
        }
    }
    return result;
}

_INLINE_ String string32_to_string(const String32& str)
{
    std::string out;
    // Have same char
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] <= 0x7F) {
            out += (char)str[i];
        }
        else if (str[i] <= 0x7FF) {
            out += 0xC0 | (char)((str[i] >> 6) & 0x1F);
            out  += 0x80 | (char)(str[i] & 0x3F);
        }
        else if (str[i] <= 0xFFFF) {
            out += 0xE0 | (char)((str[i] >> 12) & 0x0F);
            out += 0x80 | (char)((str[i] >> 6) & 0x3F);
            out += 0x80 | (char)(str[i] & 0x3F);
        }
        else if (str[i] <= 0x10FFFF) {
            out += 0xF0 | (char)((str[i] >> 18) & 0x0F);
            out += 0x80 | (char)((str[i] >> 12) & 0x3F);
            out += 0x80 | (char)((str[i] >> 6) & 0x3F);
            out += 0x80 | (char)(str[i] & 0x3F);
        }
        else {
            throw std::runtime_error("Invalid UTF-32 String");
        }
    }
    return out;
}


#endif // STRING_UTILS_H