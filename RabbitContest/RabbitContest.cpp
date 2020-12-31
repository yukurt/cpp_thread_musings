#include "Rabbit.h"
#include "RabbitContest.h"
#include "RabbitContestParams.h"

RabbitContest::RabbitContest(RabbitContestParams& contestParams_)
	: contestParams(contestParams_)
{
}

void RabbitContest::startRace()
{
	for (auto& rabbit : contestParams.rabbits)
	{
		threads.emplace_back(&Rabbit::runRace, rabbit);
	}

	for (auto& thr : threads)
	{
		thr.join();
	}
}
