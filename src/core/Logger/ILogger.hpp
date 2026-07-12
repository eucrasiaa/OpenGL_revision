#pragma once
#include <string_view>
enum class LogLevel {
  Misc,
  Info,
  Debug,
  Trace,
  Warning,
  Error,
  Critical,
};
class ILogger { 
public:
    virtual ~ILogger() = default;
    virtual void log(LogLevel level, std::string_view message) = 0;
};
