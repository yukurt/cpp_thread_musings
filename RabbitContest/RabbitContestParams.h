#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "Rabbit.h"

struct RabbitContestParams
{
	RabbitContestParams
	(
		  int raceTarget_
		, std::mutex& printMutex_
		, LogLevel logLevel_
		, std::vector<Rabbit>& rabbits_
	)
		: raceTarget(raceTarget_)
		, printMutex(printMutex_)
		, logLevel(logLevel_)
		, rabbits(rabbits_)
	{}

	int raceTarget;
	std::mutex& printMutex;
	LogLevel logLevel = LogLevel::INFO;
	std::vector<Rabbit>& rabbits;
	std::mutex finishMutex;
	std::atomic<int> winnerRabbitId = -1;
	std::condition_variable cvFinish;
	std::chrono::duration<int, std::milli> monitorInterval{ 5000 };
	bool enableRaceMonitoring = true;
};
