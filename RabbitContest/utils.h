#pragma once

#include <functional>
#include <mutex>

enum class LogLevel
{
	  FATAL = 0
	, ERROR = 1
	, WARN  = 2
	, INFO  = 3
	, DEBUG = 4
};

std::string getCurrentTimestamp();
std::string toString(LogLevel logLevel);
void printMessage
(
	  LogLevel actualLogLevel
	, LogLevel intendedLogLevel
	, std::mutex& printMutex
	, std::function<std::string()> f
);