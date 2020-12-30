#pragma once

#include <mutex>
#include <condition_variable>

struct RabbitContestParams
{
	RabbitContestParams(int raceTarget, std::mutex& printMutex)
		: raceTarget(raceTarget)
		, printMutex(printMutex)
	{}

	int raceTarget;
	std::mutex& printMutex;
	std::mutex finishMutex;
	int winnerRabbitId = -1;
	std::condition_variable cvFinish;
};
