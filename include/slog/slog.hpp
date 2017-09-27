#pragma once

#include <string>

namespace slog {

enum class LogLevel {
  Error,
  Warning,
  Info,
  Debug,
};

class Logger {
public:
  void log(LogLevel level, const std::string &category,
           const std::string &message);
};

} // namespace slog
