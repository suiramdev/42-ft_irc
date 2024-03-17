#pragma once

#include <ctime>
#include <iostream>
#include <string>

#define REGULAR_BLACK "\e[0;30m"
#define REGULAR_RED "\e[0;31m"
#define REGULAR_GREEN "\e[0;32m"
#define REGULAR_YELLOW "\e[0;33m"
#define REGULAR_BLUE "\e[0;34m"
#define REGULAR_MAGENTA "\e[0;35m"
#define REGULAR_CYAN "\e[0;36m"
#define REGULAR_WHITE "\e[0;37m"
#define REGULAR_GRAY "\e[0;90m"

#define BOLD_BLACK "\e[1;30m"
#define BOLD_RED "\e[1;31m"
#define BOLD_GREEN "\e[1;32m"
#define BOLD_YELLOW "\e[1;33m"
#define BOLD_BLUE "\e[1;34m"
#define BOLD_MAGENTA "\e[1;35m"
#define BOLD_CYAN "\e[1;36m"
#define BOLD_WHITE "\e[1;37m"
#define BOLD_GRAY "\e[1;90m"

#define UNDERLINE_BLACK "\e[4;30m"
#define UNDERLINE_RED "\e[4;31m"
#define UNDERLINE_GREEN "\e[4;32m"
#define UNDERLINE_YELLOW "\e[4;33m"
#define UNDERLINE_BLUE "\e[4;34m"
#define UNDERLINE_MAGENTA "\e[4;35m"
#define UNDERLINE_CYAN "\e[4;36m"
#define UNDERLINE_WHITE "\e[4;37m"
#define UNDERLINE_GRAY "\e[4;90m"

#define BACKGROUND_BLACK "\e[40m"
#define BACKGROUND_RED "\e[41m"
#define BACKGROUND_GREEN "\e[42m"
#define BACKGROUND_YELLOW "\e[43m"
#define BACKGROUND_BLUE "\e[44m"
#define BACKGROUND_MAGENTA "\e[45m"
#define BACKGROUND_CYAN "\e[46m"
#define BACKGROUND_WHITE "\e[47m"
#define BACKGROUND_GRAY "\e[100m"

#define RESET "\e[0m"

class Logger {
private:
  static void printTime() {
    time_t now = time(0);
    tm *localTime = localtime(&now);

    std::cout << BOLD_WHITE << localTime->tm_hour << ":" << localTime->tm_min
              << ":" << localTime->tm_sec << RESET << " ";
  }

public:
  static void info(const std::string &message) {
    printTime();
    std::cout << BACKGROUND_BLUE << "INFO" << REGULAR_BLUE << " " << message
              << RESET << std::endl;
  }

  static void error(const std::string &message) {
    printTime();
    std::cout << BACKGROUND_RED << "ERROR" << REGULAR_RED << " " << message
              << RESET << std::endl;
  }

  static void warning(const std::string &message) {
    printTime();
    std::cout << BACKGROUND_YELLOW << "WARNING" << REGULAR_YELLOW << " "
              << message << RESET << std::endl;
  }

  static void in(const std::string &message) {
    printTime();
    std::cout << BACKGROUND_GRAY << "IN" << REGULAR_WHITE << " " << message
              << RESET << std::endl;
  }

  static void out(const std::string &message) {
    printTime();
    std::cout << BACKGROUND_BLACK << "OUT" << REGULAR_GRAY << " " << message
              << RESET << std::endl;
  }
};
