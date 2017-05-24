#pragma once
#include <iostream>
#include <string>

using namespace std;

class Log {
public:
  static void info(string str) {
    write_line("[INFO] " + str);
  }

  static void warn(string str) {
    write_line("[WARN] " + str);
  }

  static void error(string str) {
    write_line("[ERROR] " + str);
  }

private:
  static void write_line(string str) {
#ifndef LOGGING
    return;
#endif
    cout << str << endl;
  }
};
