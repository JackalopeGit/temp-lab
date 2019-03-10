#pragma once

#include "../templates/array/smart.h"

#include <cinttypes>
#include <cstring>

struct String
{
    [[nodiscard]] inline bool init(const char *src);
    [[nodiscard]] inline bool init(const char *src, uint32_t length);
                  inline void destroy();

    [[nodiscard]] inline char& operator [] (uint32_t index);
    [[nodiscard]] inline char  operator [] (uint32_t index) const;

    [[nodiscard]] inline uint32_t length() const;

    operator       char* () { return data.p_array; }
    operator const char* () const { return data.p_array; }

    [[nodiscard]] inline       char& last()       { return data[data.size-2]; }
    [[nodiscard]] inline const char& last() const { return data[data.size-2]; }

    mtl::array::resizable<char, uint32_t> data;
};


[[nodiscard]] inline bool String::init(const char *src)
{
    uint32_t length = uint32_t(strlen(src));
    return init(src, length);
}

[[nodiscard]] inline bool String::init(const char *src, uint32_t length)
{
    if (not data.init(length+1))
        return false;

    memcpy(data.p_array, src, length);
    data.p_array[length] = 0;

    return true;
}

inline void String::destroy()
{
    data.destroy();
}

[[nodiscard]] inline char& String::operator [] (uint32_t index)
{
    return data.p_array[index];
}

[[nodiscard]] inline char  String::operator [] (uint32_t index) const
{
    return data.p_array[index];
}

[[nodiscard]] inline uint32_t String::length() const
{
    return data.size - 1;
}

