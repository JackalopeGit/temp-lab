#pragma once

#include <cstdio>

#include "time.h"

namespace io
{
    using Color = const char*;

    constexpr const Color BLACK        = "0;30";
    constexpr const Color DARK_GRAY    = "1;30";
    constexpr const Color RED          = "0;31";
    constexpr const Color GREEN        = "0;32";
    constexpr const Color BROWN_ORANGE = "0;33";
    constexpr const Color BLUE         = "0;34";
    constexpr const Color PURPLE       = "0;35";
    constexpr const Color CYAN         = "0;36";
    constexpr const Color LIGHT_GRAY   = "0;37";
    constexpr const Color LIGHT_RED    = "1;31";
    constexpr const Color LIGHT_GREEN  = "1;32";
    constexpr const Color Yellow       = "1;33";
    constexpr const Color LIGHT_BLUE   = "1;34";
    constexpr const Color LIGHT_PURPLE = "1;35";
    constexpr const Color LIGHT_CYAN   = "1;36";
    constexpr const Color WHITE        = "1;37";

    // begin "\033[" + <COLOR> + "m"

    // end   "\033[0m"

    inline void out(const char* message)
    {
        printf("%s", message);
    }

    inline void out(int number)
    {
        printf("%i", number);
    }
    inline void out(int number, Color color_str)
    {
        printf("\033[%sm%i\033[0m", color_str, number);
    }
    inline void out_n(int number)
    {
        printf("%i\n", number);
    }
    inline void out_n(int number, Color color_str)
    {
        printf("\033[%sm%i\033[0m\n", color_str, number);
    }

    using Format = const char[];

    template<Format format, typename... Args>
    inline void out(Color color_str, Args ... args)
    {
        printf("\033[%sm", color_str);
        printf(format, args ...);
        printf("\033[0m");
    }


    namespace FORMAT
    {
        constexpr Format S     = "%s";
        constexpr Format S_S   = "%s%s";
        constexpr Format S_S_N = "%s%s\n";
        constexpr Format I_S_N = "%i%s\n";
        constexpr Format S_I   = "%s%i";
        constexpr Format S_N   = "%s\n";

        constexpr Format LOG_1  = "%10.6f ";
        constexpr Format LOG_2  = "[%7.3f ms] ";
    }


    inline void log(const char *m, double time_start, Color color = io::BLACK)
    {
#if 1
        (void)m;(void)(time_start);(void)(color);
#else
        double current = getTime();
        static double last_event_time = current;
        io::out<FORMAT::LOG_1>(io::BROWN_ORANGE, current - time_start);
        auto ms = (current - last_event_time) * 1000.;

        io::Color color_;
        if (ms > 10)
            color_ = io::RED;
        else
            color_ = io::BLUE;

        io::out<FORMAT::LOG_2>(color_, ms);
        if (color != BLACK)
            io::out<FORMAT::S_N>(color, m);
        else
            printf("%s\n",m);
        last_event_time = current;
#endif
    }
}

