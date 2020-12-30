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
		, std::chrono::duration<unsigned int> const& timeTick
		, std::mutex& printMtx
		, std::size_t numRabbits
	);

	void startRace();

private:
	int raceTarget;
	const std::chrono::duration<unsigned int> raceTimeTick;
	std::mutex& printMutex;

	std::vector<Rabbit> rabbits;
	std::vector<std::thread> threads;
};
