#include <ctime>
#include <string>
#include <chrono>

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