#pragma once



#include "Logger/ILogger.hpp" 
#include <print>
#include <unordered_map>
#include <string>
#include "ansi_min.hpp"




#ifdef MIN_LOG_WARN
    constexpr LogLevel MinLogLevel = LogLevel::Warning;
#elif MIN_LOG_ERROR
    constexpr LogLevel MinLogLevel = LogLevel::Error;
#else 
    constexpr LogLevel MinLogLevel = LogLevel::Info;
#endif


class LoggerBasic : public ILogger {
  public:

    void log(LogLevel level, std::string_view message) override {
      printToConsole(level, message);
      if (level == LogLevel::Trace) {
        traceHistory_[nextTraceId_++] = std::string(message);
      }
    }
    const std::unordered_map<int, std::string>& getTraceHistory() const {
      return traceHistory_;
    } 
  private:
    int nextTraceId_ = 0;
    std::unordered_map<int, std::string> traceHistory_;
    void printToConsole(LogLevel level, std::string_view message) {
      if (level < MinLogLevel){
        return;
      }
      using enum LogLevel; 
      switch (level){
        case LogLevel::Critical:
          std::print("{}[Critical] {} {}\n",term::fg::RED, message,  term::format::RESET); 
          break;
        case LogLevel::Error:
          std::print("{}[Error] {} {}\n",term::fg::YELLOW,  message, term::format::RESET); 
          break;
        case LogLevel::Warning:
          std::print("{}[Warning] {}{}\n",term::fg::CYAN,  message, term::format::RESET); 
          break;
        case LogLevel::Trace:
          std::print("[Trace {}] {}\n", nextTraceId_, message); 
          break;
        case LogLevel::Debug:
          std::print("[Debug] {}\n", message); 
          break;
        case LogLevel::Info: 
          std::print("{}[Info] {} {}\n",term::fg::BLUE, message, term::format::RESET); 
          break;
        case LogLevel::Misc:
          std::print("[Misc] {}\n", message); 
          break;
      }
    }
};


  // logger_->log(LogLevel::Critical, " Critical");   
  // logger_->log(LogLevel::Warning, " Warning");   
  // logger_->log(LogLevel::Error, " Error");   
  // logger_->log(LogLevel::Misc, " Misc");   
  // logger_->log(LogLevel::Trace, " Trace");   
  // logger_->log(LogLevel::Info, " Info");   
  // logger_->log(LogLevel::Debug, " Debug");   
  // logger_->log(LogLevel::Trace, " Trace 2");   
  //
  // logger_->log(LogLevel::Trace, " Trace 3");   
  // logger_->log(LogLevel::Trace, " Trace 4");   
  // std::print("{}",static_cast<LoggerBasic*>(logger_)->getTraceHistory());
