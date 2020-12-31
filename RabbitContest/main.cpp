#include <mutex>
#include <chrono>
#include <vector>

#include "RabbitParams.h"
#include "RabbitContestParams.h"
#include "RabbitContest.h"
#include "utils.h"
#include "main.h"

int main()
{
    const int raceTarget = 70;
    const int numRabbits = 8;
    const int numRaces = 2;

    std::mutex printMutex;

    std::vector<Rabbit> rabbits;
    RabbitContestParams contestParams(raceTarget, printMutex,
        LogLevel::INFO, rabbits);
    std::vector<RabbitParams> rabbitParamsCollection =
        createRabbitParamsCollection(numRabbits, printMutex);

    createRabbits(rabbitParamsCollection, rabbits, contestParams);

    startRaces(contestParams, numRaces);

    return 0;
}

std::vector<RabbitParams> createRabbitParamsCollection
    (int numRabbits, std::mutex& printMutex)
{
    std::vector<RabbitParams> collection;

    for (int i = 0; i < numRabbits; ++i)
    {
        collection.emplace_back(i, printMutex, LogLevel::INFO);
    }

    return collection;
}

void createRabbits(std::vector<RabbitParams>& rabbitParamsCollection, 
    std::vector<Rabbit>& rabbits, RabbitContestParams& contestParams)
{
    for (auto& rabbitParams : rabbitParamsCollection)
    {
        rabbits.emplace_back(rabbitParams, contestParams);
    }
}

void startRaces(RabbitContestParams& contestParams, const int& numRaces)
{
    RabbitContest contest(contestParams);
    for (int i = 0; i < numRaces; ++i)
    {
        contest.startRace();
    }
}
