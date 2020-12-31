#include <ctime>
#include <string>
#include <chrono>
#include <mutex>
#include <iostream>

#include "utils.h"

std::string getCurrentTimestamp()
{
	using std::chrono::system_clock;
	auto currentTime = std::chrono::system_clock::now();
	char buffer[80];

	auto transformed = currentTime.time_since_epoch().count() / 1000000;

	auto millis = transformed % 1000;

	std::time_t tt;
	tt = system_clock::to_time_t(currentTime);
	struct tm timeinfo;
	localtime_s(&timeinfo, &tt);
	strftime(buffer, 80, "%H:%M:%S", &timeinfo);
	sprintf_s(buffer, "%s:%03d", buffer, (int)millis);

	return std::string(buffer);
}

std::string toString(LogLevel logLevel)
{
	switch (logLevel)
	{
	case LogLevel::FATAL:
		return "FATAL";
	case LogLevel::ERROR:
		return "ERROR";
	case LogLevel::WARN:
		return "WARN ";
	case LogLevel::INFO:
		return "INFO ";
	case LogLevel::DEBUG:
		return "DEBUG";
	default:
		return "UNKNOWN!!!";
	}
}

void printMessage
(
	LogLevel actualLogLevel
	, LogLevel intendedLogLevel
	, std::mutex& printMutex
	, std::function<std::string()> f
)
{
	if (actualLogLevel >= intendedLogLevel)
	{
		std::lock_guard<std::mutex> lock(printMutex);

		std::cout
			<< toString(intendedLogLevel)
			<< ": "
			<< getCurrentTimestamp()
			<< ": "
			<< f()
			<< std::endl;
	}
}