#pragma once

#include <chrono>
#include <mutex>

#include "utils.h"

struct RabbitParams
{
	RabbitParams
	(
		  unsigned int id_
		, std::mutex& printMutex_
		, LogLevel logLevel_
	)
		: id(id_)
		, printMutex(printMutex_)
		, logLevel(logLevel_)
	{}

	unsigned int id = -1;
	std::mutex& printMutex;
	LogLevel logLevel = LogLevel::INFO;
	std::chrono::duration<int, std::milli> shortestNapTime{ 500 };
	std::chrono::duration<int, std::milli> longestNapTime{ 5000 };
};