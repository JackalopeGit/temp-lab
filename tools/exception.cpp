#include "exception.h"

#include <cstdlib>

#include "io.h"

inline void exception_out(const char *message)
{
    io::out<io::FORMAT::S>(io::RED, "EXCEPTION: ");
    printf("%s\n",message);
}

[[noreturn]] void exception(const char* message)
{
    exception_out(message);
    std::fflush(stdout);
    quick_exit(1);
}

[[noreturn]] void exception(const char* message, const char* error)
{
    exception_out(message);
    error_output(error);
    quick_exit(1);
}

[[noreturn]] void exception(const char* message, int errorNo)
{
    exception_out(message);
    quick_exit(errorNo);
}

void error_output(const char* message, int err)
{
    io::out<io::FORMAT::S_I>(io::RED, "ERROR: ", err);
    printf("%s\n",message);
    std::fflush(stdout);
}


void error_output(const char* message)
{
    io::out<io::FORMAT::S>(io::RED, "ERROR: ");
    printf("%s\n",message);
    std::fflush(stdout);
}


void warning_output(const char* message)
{
    io::out<io::FORMAT::S_S_N>(io::Yellow, "WARNING: ", message);
    std::fflush(stdout);
}
