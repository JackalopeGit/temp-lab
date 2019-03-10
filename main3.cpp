
#include "misc.h"

constexpr static const char* ref[] =
{
    "Invalid floating constant"
};

enum Refs
{
    FLOATING_CONSTANT_ERROR
};

inline void error(uint32_t line_i, uint32_t char_i, Refs refNo)
{
    printf("Error at line %u:%u - %s\n", line_i, char_i,  ref[refNo]);
}

enum State { SPACE, SIGN, INTEGER, DOT, FRACTION, COUNT, ERROR = -1 };

struct Chars
{
    [[nodiscard]] inline char operator[](size_t i) const { return chars[i]; }

    const char *chars;
    size_t      count;
};

constexpr static const Chars CHARS[State::COUNT] =
{
    { " \n",        2  },
    { "+-",         2  },
    { "0123456789", 10 },
    { ".",          1  },
    { "0123456789", 10 }
};

struct Vertex
{
    constexpr static Vertex build(State src, State dst)
    {
        return { src, dst, CHARS[dst] };
    }
    State src;
    State dst;
    Chars values;
};

constexpr static const Vertex VERTICES[] =
{
    Vertex::build(SPACE,    SPACE   ),
    Vertex::build(SPACE,    SIGN    ),
    Vertex::build(SPACE,    INTEGER ),
    Vertex::build(SIGN,     INTEGER ),
    Vertex::build(INTEGER,  SPACE   ),
    Vertex::build(INTEGER,  INTEGER ),
    Vertex::build(INTEGER,  DOT     ),
    Vertex::build(DOT,      FRACTION),
    Vertex::build(DOT,      SPACE   ),
    Vertex::build(FRACTION, SPACE   ),
    Vertex::build(FRACTION, FRACTION),
    Vertex::build(ERROR,    SPACE   ),
};


[[nodiscard]] inline State update_state(State current, char c)
{
    for (auto &ver : VERTICES)
    {
        if (ver.src == current)
        {
            auto &chars = ver.values;
            for (size_t i = 0; i < chars.count; ++i)
                if (c == chars[i])
                    return ver.dst;
        }
    }
    return State::ERROR;
}

int main()
{
    String file_data;
    if (not readFileToStr("basic_reals", &file_data))
        return -1;

    State state = State::SPACE;

    uint32_t line_i = 1, char_i = 1;
    for (uint32_t i = 0; i <= file_data.length(); ++i, ++char_i)
    {
        auto new_state = update_state(state, file_data[i]);
        if (new_state == State::ERROR && state != State::ERROR)
            error(line_i, char_i, Refs::FLOATING_CONSTANT_ERROR);
        state = new_state;

        if (file_data[i] == '\n')
            ++line_i, char_i = 0;
    }
}

