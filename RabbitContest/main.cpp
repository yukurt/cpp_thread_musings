#include <iostream>
#include <mutex>
#include <chrono>

#include "RabbitContest.h"

int main()
{
    std::mutex printMutex;

    using namespace std::chrono_literals;
    std::chrono::duration<unsigned int> raceTimeTick = 1s;

    const int raceTarget = 70;

    const int numRabbits = 8;

    RabbitContest contest(raceTarget, printMutex, numRabbits);
    contest.startRace();
}