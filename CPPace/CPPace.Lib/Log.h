#pragma once
#include <iostream>
#include <string>
#include <cstdarg>

using namespace std;

class Log {
public:
  static void info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf_wrapper(string("[INFO] ").append(format).c_str(), args);
    va_end(args);
  }

  static void warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf_wrapper(string("[WARN] ").append(format).c_str(), args);
    va_end(args);
  }

  static void error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf_wrapper(string("[ERROR] ").append(format).c_str(), args);
    va_end(args);
  }

private:
  static void printf_wrapper(const char* format, va_list args) {
#ifndef LOGGING
    return;
#endif
    vprintf(format, args);
    printf("\n");
  }
};
