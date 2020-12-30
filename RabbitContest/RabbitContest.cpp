#include "RabbitContest.h"

RabbitContest::RabbitContest
(
	  int target
	, std::chrono::duration<unsigned int> const& timeTick
	, std::mutex& printMtx
	, std::size_t numRabbits
)
	: raceTarget(target)
	, raceTimeTick(timeTick)
	, printMutex(printMtx)
{
	for (std::size_t i = 0; i < numRabbits; ++i)
	{
		rabbits.emplace_back(i, raceTarget, raceTimeTick, printMutex);
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
