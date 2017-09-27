#include "slog/slog.hpp"

int main() {
  slog::Logger logger;

  logger.log(slog::LogLevel::Error, "main", "a log message");
  return 0;
}
