#pragma once

#include <cstdio>
#include <cinttypes>

struct File
{
    using Offset = off_t;

    constexpr static const size_t NAME_LENGTH_MAX = 256;

    enum ModeFlags : uint8_t
    {
        READ_BIT         = 0b00000001,
        WRITE_BIT        = 0b00000010,
        CLEAN_BIT        = 0b00000100,
        WRITE_TO_END_BIT = 0b00001000,
        TEXT_BIT         = 0b00010000,
    };

    struct Mode
    {
        constexpr inline Mode(ModeFlags);
        char str[4];
    };


    [[nodiscard]] inline bool open(const char *path, Mode mode);
    [[nodiscard]] inline bool open(const char *path, ModeFlags flags);
                  inline bool close();

    [[nodiscard]] inline bool   position_set (Offset index);
    [[nodiscard]] inline bool   position_set_end();
    [[nodiscard]] inline bool   position_move(Offset diff);
    [[nodiscard]] inline Offset position();

    [[nodiscard]] inline bool   position_at_end();
    [[nodiscard]] inline bool   position_at_end_locked();

                  template<typename Data>
    [[nodiscard]] inline bool read(size_t count, Data *p_dst);
                  template<typename Data>
    [[nodiscard]] inline bool read(Data *p_dst);
                  template<typename Data>
    [[nodiscard]] inline bool read_bytes(Data *p_dst, size_t byte_count);

    [[nodiscard]] inline bool read_string(int limit, char *dst);

                  template<typename Data>
    [[nodiscard]] inline bool read_locked(size_t count, Data &dst);
                  template<typename Data>
    [[nodiscard]] inline bool read_locked(Data *p_dst);
                  template<typename Data>
    [[nodiscard]] inline bool read_bytes_locked(Data *p_dst, size_t byte_count);

    [[nodiscard]] inline bool read_string_locked(int limit, char *dst);

                  template<typename Data>
    [[nodiscard]] inline bool write(size_t count, Data *p_dst);
                  template<typename Data>
    [[nodiscard]] inline bool write(Data *p_dst);
                  template<typename Data>
    [[nodiscard]] inline bool write_bytes(Data *p_dst, size_t byte_count);

    [[nodiscard]] inline bool write_string(char *dst);

                  template<typename Data>
    [[nodiscard]] inline bool write_locked(size_t count, Data &dst);
                  template<typename Data>
    [[nodiscard]] inline bool write_locked(Data *p_dst);

    [[nodiscard]] inline bool write_string_locked(char *dst);

    FILE *p_file;
};

[[nodiscard]] inline File::ModeFlags operator | (File::ModeFlags _1, File::ModeFlags _2)
{
    return File::ModeFlags(uint8_t(_1) | _2);
}

/* http://www.cplusplus.com/reference/cstdio/fopen/

"r"	READ_BIT
read: Open file for input operations. The file must exist.

"w"	WRITE_BIT CLEAN_BIT
write: Create an empty file for output operations.
 If a file with the same name already exists,
 its contents are discarded and the file is treated as a new empty file.

"a"	WRITE_BIT END_BIT
append: Open file for output at the end of a file.
Output operations always write data at the end of the file, expanding it.
Repositioning operations (fseek, fsetpos, rewind) are ignored.
The file is created if it does not exist.

"r+"	READ_BIT WRITE_BIT
read/update: Open a file for update (both for input and output). The file must exist.

"w+"	READ_BIT WRITE_BIT CLEAN_BIT
write/update: Create an empty file and open it for update (both for input and output).
If a file with the same name already exists its contents are discarded
and the file is treated as a new empty file.

"a+"	READ_BIT WRITE_BIT END_BIT
append/update: Open a file for update (both for input and output)
with all output operations writing data at the end of the file.
Repositioning operations (fseek, fsetpos, rewind) affects the next input operations,
but output operations move the position back to the end of file.
The file is created if it does not exist.

*/
constexpr inline File::Mode::Mode(ModeFlags flags) : str()
{
    int str_size = 0;
    if (flags & READ_BIT)
    {
        if (flags & CLEAN_BIT)
        {
            str[0] = 'w';
            str[1] = '+';
            str_size = 2;
        }
        else if (flags & WRITE_TO_END_BIT)
        {
            str[0] = 'a';
            str[1] = '+';
            str_size = 2;
        }
        else if (flags & WRITE_BIT)
        {
            str[0] = 'r';
            str[1] = '+';
            str_size = 2;
        }
        else
        {
            str[0] = 'r';
            str_size = 1;
        }
    }
    else if (flags & WRITE_TO_END_BIT)
    {
        str[0] = 'a';
        str_size = 1;
    }
    else if (flags & CLEAN_BIT)
    {
        str[0] = 'w';
        str_size = 1;
    }

    if (not (flags & TEXT_BIT))
        str[str_size++] = 'b';
}


[[nodiscard]] inline bool File::open(const char *path, Mode mode)
{
    p_file = fopen(path, mode.str);
    return p_file != nullptr;
}

[[nodiscard]] inline bool File::open(const char *path, ModeFlags flags)
{
    Mode mode(flags);
    return open(path, mode);
}

inline bool File::close()
{
    return fclose(p_file) == 0;
}


[[nodiscard]] inline bool File::position_set (Offset index)
{
    return fseek(p_file, index, SEEK_SET) == 0;
}
[[nodiscard]] inline bool File::position_set_end ()
{
    return fseek(p_file, 0, SEEK_END) == 0;
}
[[nodiscard]] inline bool File::position_move(Offset diff)
{
    return fseek(p_file, diff, SEEK_CUR) == 0;
}

[[nodiscard]] inline File::Offset File::position()
{
    return ftell(p_file);
}
[[nodiscard]] inline bool File::position_at_end()
{
    return feof_unlocked(p_file) == EOF;
}
[[nodiscard]] inline bool File::position_at_end_locked()
{
    return feof(p_file) == EOF;
}



template<typename Data>
[[nodiscard]] inline bool File::read(size_t count, Data *p_dst)
{
    return fread_unlocked(p_dst, sizeof (Data), count, p_file) == count;
}

template<typename Data>
[[nodiscard]] inline bool File::read(Data *p_dst)
{
    return fread_unlocked(p_dst, sizeof (Data), 1, p_file) == 1;
}

template<>
[[nodiscard]] inline bool File::read<char>(char *p_dst)
{
    int c = fgetc_unlocked(p_file);
    if (c == EOF)
        return false;
    *p_dst = char(c);
    return true;
}

template<typename Data>
[[nodiscard]] inline bool File::read_bytes(Data *p_dst, size_t byte_count)
{
    return fread_unlocked(p_dst, byte_count, 1, p_file) == 1;
}

template<>
[[nodiscard]] inline bool File::read_locked<char>(char *p_dst)
{
    int c = fgetc(p_file);
    if (c == EOF)
        return false;
    *p_dst = char(c);
    return true;
}

template<typename Data>
[[nodiscard]] inline bool File::read_locked(size_t count, Data &dst)
{
    return fread(&dst, sizeof (Data), count, p_file) == count;
}

template<typename Data>
[[nodiscard]] inline bool File::read_bytes_locked(Data *p_dst, size_t byte_count)
{
    return fread(p_dst, byte_count, 1, p_file) == 1;
}

[[nodiscard]] inline bool File::read_string(int limit, char *dst)
{
    return fgets_unlocked(dst, limit, p_file) != nullptr;
}
[[nodiscard]] inline bool File::read_string_locked(int limit, char *dst)
{
    return fgets(dst, limit, p_file) != nullptr;
}

template<typename Data>
[[nodiscard]] inline bool File::write(size_t count, Data *p_dst)
{
    return fwrite_unlocked(p_dst, sizeof (Data), count, p_file) == count;
}
template<typename Data>
[[nodiscard]] inline bool File::write(Data *p_dst)
{
    return fwrite_unlocked(p_dst, sizeof (Data), 1, p_file) == 1;
}
template<typename Data>
[[nodiscard]] inline bool File::write_bytes(Data *p_dst, size_t byte_count)
{
    return fwrite_unlocked(p_dst, byte_count, 1, p_file) == 1;
}

[[nodiscard]] inline bool File::write_string(char *dst)
{
    return fputs_unlocked(dst, p_file);
}

template<typename Data>
[[nodiscard]] inline bool File::write_locked(size_t count, Data &dst)
{
    return fwrite(dst, sizeof (Data), count, p_file) == count;
}
template<typename Data>
[[nodiscard]] inline bool File::write_locked(Data *p_dst)
{
    return fwrite(p_dst, sizeof (Data), 1, p_file) == 1;
}

[[nodiscard]] inline bool File::write_string_locked(char *dst)
{
    return fputs(dst, p_file);
}
