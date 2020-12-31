#include <mutex>
#include <chrono>
#include <vector>

#include "RabbitParams.h"
#include "RabbitContestParams.h"
#include "RabbitContest.h"

int main()
{
    const int raceTarget = 70;
    const int numRabbits = 8;

    std::mutex printMutex;

    std::vector<Rabbit> rabbits;
    RabbitContestParams contestParams(raceTarget, printMutex, rabbits);

    std::vector<RabbitParams> rabbitParamsCollection;
    for (int i = 0; i < numRabbits; ++i)
    {
        rabbitParamsCollection.emplace_back(i);
    }

    for (auto& rabbitParams : rabbitParamsCollection)
    {
        rabbits.emplace_back(rabbitParams, contestParams);
    }

    RabbitContest contest(contestParams);
    contest.startRace();
}