#pragma once

#include <mutex>
#include <vector>
#include "Rabbit.h"
#include <thread>

class RabbitContest
{
public:
	RabbitContest
	(
		  int target
		, std::mutex& printMtx
		, std::size_t numRabbits
	);

	void startRace();

private:
	int raceTarget;
	std::mutex& printMutex;

	std::vector<Rabbit> rabbits;
	std::vector<std::thread> threads;
};
