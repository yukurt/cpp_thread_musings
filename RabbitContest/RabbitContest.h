#pragma once

#include <mutex>
#include <vector>
#include "Rabbit.h"
#include <thread>
#include <condition_variable>

class RabbitContest
{
public:
	RabbitContest
	(
		  int raceTarget
		, std::mutex& printMutex
		, std::size_t numRabbits
	);

	void startRace();

private:
	RabbitContestParams contestParams;

	std::vector<Rabbit> rabbits;
	std::vector<std::thread> threads;
};
