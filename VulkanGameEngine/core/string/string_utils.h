#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "string_types.h"
#include "../typedefs.h"
#include <stdexcept>
#include <string_view>
#include <boost/locale.hpp>

//X TODO : Create set of method that hashes the strings to size_t


size_t hash_string(const String& str);

_INLINE_ std::wstring string_to_wstr(const String& str)
{
    return boost::locale::conv::utf_to_utf<wchar_t>(str);
}

_INLINE_ std::wstring string16_to_wstr(const String16& str)
{
    return boost::locale::conv::utf_to_utf<wchar_t>(str);

}

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

namespace impl
{
    /// Base declaration of our constexpr string_view concatenation helper
    template <std::string_view const&, typename, std::string_view const&, typename>
    struct concat;

    /// Specialisation to yield indices for each char in both provided string_views,
    /// allows us flatten them into a single char array
    template <std::string_view const& S1,
        std::size_t... I1,
        std::string_view const& S2,
        std::size_t... I2>
    struct concat<S1, std::index_sequence<I1...>, S2, std::index_sequence<I2...>>
    {
        static constexpr const char value[]{ S1[I1]..., S2[I2]..., 0 };
    };
} // namespace impl

/// Base definition for compile time joining of strings
template <std::string_view const&...> struct join;

/// When no strings are given, provide an empty literal
template <>
struct join<>
{
    static constexpr std::string_view value = "";
};

/// Base case for recursion where we reach a pair of strings, we concatenate
/// them to produce a new constexpr string
template <std::string_view const& S1, std::string_view const& S2>
struct join<S1, S2>
{
    static constexpr std::string_view value =
        impl::concat<S1,
        std::make_index_sequence<S1.size()>,
        S2,
        std::make_index_sequence<S2.size()>>::value;
};

/// Main recursive definition for constexpr joining, pass the tail down to our
/// base case specialisation
template <std::string_view const& S, std::string_view const&... Rest>
struct join<S, Rest...>
{
    static constexpr std::string_view value =
        join<S, join<Rest...>::value>::value;
};

/// Join constexpr string_views to produce another constexpr string_view
template <std::string_view const&... Strs>
static constexpr auto join_v = join<Strs...>::value;


#endif // STRING_UTILS_H