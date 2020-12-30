#include <iostream>
#include <string>
#include <thread>
#include <ctime>
#include <iomanip>
#include <random>

#include "Rabbit.h"
#include "utils.h"

Rabbit::Rabbit(
	  unsigned int rabbitId
	, int target
	, std::mutex& printMtx
)
	: randomGenerator((std::random_device())())
	, hopRandomizer{ 0, 9 }
	, napRandomizer{ 500, 5000 }
	, id(rabbitId)
	, raceTarget(target)
	, printMutex(printMtx)
{
}

int Rabbit::getHop()
{
	auto random = hopRandomizer(randomGenerator);

	int hop = 0;
	switch (random)
	{
	case 0:
	case 1:
		hop = 0;
		break;
	case 2:
	case 3:
		hop = 9;
		break;
	case 4:
		hop = -12;
		break;
	case 5:
	case 6:
	case 7:
		hop = 1;
		break;
	case 8:
	case 9:
		hop = 2;
		break;
	default:
		printMessage("ERROR: " + std::string(__func__) + 
			": " + std::to_string(id) + ": Illegal random number = " + 
			std::to_string(random));
	}

	return hop;
}

void Rabbit::takeNap()
{
	auto napDuration = napRandomizer(randomGenerator);
	std::this_thread::sleep_for(std::chrono::milliseconds(napDuration));
}

void Rabbit::runRace()
{
	currentLocation = 0;

	while (currentLocation < raceTarget)
	{
		int hop = getHop();
		currentLocation += hop;

		if (currentLocation < 0)
		{
			currentLocation = 0;
		}

		printMessage("hop = " + std::to_string(hop) +
			" location = " + std::to_string(currentLocation));

		takeNap();
	}

	printMessage("Completed race");
}

void Rabbit::printMessage(std::string const& msg) const
{
	std::lock_guard<std::mutex> lock(printMutex);

	std::cout << getCurrentTimestamp() << ": " << id
		<< ": " << msg << std::endl;
}
