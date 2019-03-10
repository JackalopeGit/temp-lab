#pragma once

[[noreturn]] void exception(const char* message);

[[noreturn]] void exception(const char* message, const char* error);

[[noreturn]] void exception(const char* message, int errorNo);



void error_output(const char* message, int err);

void error_output(const char* message);


void warning_output(const char* message);


