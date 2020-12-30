#include "RabbitContest.h"
#include "RabbitContestParams.h"

RabbitContest::RabbitContest
(
	int raceTarget
	, std::mutex& printMutex
	, std::size_t numRabbits
)
	: contestParams{ raceTarget, printMutex }
{
	for (std::size_t i = 0; i < numRabbits; ++i)
	{
		rabbits.emplace_back(i, contestParams);
	}
}

void RabbitContest::startRace()
{
	for (auto& rabbit : rabbits)
	{
		threads.emplace_back(&Rabbit::runRace, rabbit);
	}

	for (auto& thr : threads)
	{
		thr.join();
	}
}
