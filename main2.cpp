
#include "misc.h"

constexpr static const char* ref[] =
{
    "After \"11\" must be '0'"
};

enum Refs
{
    ZERO_AFTER_TWO_ONES
};

inline void error(uint32_t line_i, uint32_t char_i, Refs refNo)
{
    printf("Error at line %u:%u - %s\n", line_i, char_i,  ref[refNo]);
}

int main()
{
    String file_data;
    if (not readFileToStr("useless_language_file", &file_data))
        return -1;

    uint32_t line_i = 1, char_i = 1;
    for (uint32_t i = 2; i <= file_data.length(); ++i, ++char_i)
    {
        if (file_data[i] != '0' && file_data[i-1] == '1' && file_data[i-2] == '1')
            error(line_i, char_i, Refs::ZERO_AFTER_TWO_ONES);

        if (file_data[i] == '\n')
            ++line_i, char_i = 0;
    }
}
