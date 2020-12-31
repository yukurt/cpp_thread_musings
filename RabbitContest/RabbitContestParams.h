#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>

#include "Rabbit.h"

struct RabbitContestParams
{
	RabbitContestParams
	(
		  int raceTarget_
		, std::mutex& printMutex_
		, std::vector<Rabbit>& rabbits_
	)
		: raceTarget(raceTarget_)
		, printMutex(printMutex_)
		, rabbits(rabbits_)
	{}

	int raceTarget;
	std::mutex& printMutex;
	std::vector<Rabbit>& rabbits;
	std::mutex finishMutex;
	int winnerRabbitId = -1;
	std::condition_variable cvFinish;
};
