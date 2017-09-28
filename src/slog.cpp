#include "slog/slog.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>

#include <fmt/format.h>
#include <fmt/time.h>

namespace {

const char *levelToString(slog::LogLevel level) {
  switch (level) {
  case slog::LogLevel::Error: return "error";
  case slog::LogLevel::Warning: return "warning";
  case slog::LogLevel::Info: return "Info";
  case slog::LogLevel::Debug: return "Debug";
  }

  return "<Unknown>";
};

} // unnamed namespace

namespace slog {

void Logger::log(LogLevel level,
                 const std::string &category,
                 const std::string &message) {
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);

  fmt::print("{:%Y/%m/%d %T} | {} | {} | {}\n",
             tm,
             levelToString(level),
             category,
             message);
}

} // namespace slog
