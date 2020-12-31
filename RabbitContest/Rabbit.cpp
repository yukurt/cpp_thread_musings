#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include "Rabbit.h"
#include "RabbitParams.h"
#include "RabbitContestParams.h"
#include "utils.h"

Rabbit::Rabbit(RabbitParams& rabbitParams_, 
	RabbitContestParams& contestParams_)
	: rabbitParams(rabbitParams_)
	, contestParams(contestParams_)
	, randomGenerator((std::random_device())())
	, hopRandomizer{ 0, 9 }
	, napRandomizer{ rabbitParams.shortestNapTime.count(), 
		rabbitParams.longestNapTime.count() }
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
		printMessage(LogLevel::ERROR, std::string(__func__) + 
			": Illegal random number = " + std::to_string(random));
	}

	return hop;
}

void Rabbit::takeNap()
{
	auto napDuration = napRandomizer(randomGenerator);

	std::unique_lock<std::mutex> lock(contestParams.finishMutex);
	someoneElseWon = contestParams.cvFinish.wait_for(lock, 
		std::chrono::milliseconds(napDuration),
		[&contestParams = contestParams]() 
		{ 
			return contestParams.winnerRabbitId >= 0; 
		});
}

std::string Rabbit::generateHopMessage(int hop) const
{
	std::ostringstream oss;
	oss << "hop = " << std::right << std::setw(3) << hop
		<< " location = " << std::setw(2) << currentLocation;
	return oss.str();
}

void Rabbit::makeHop()
{
	int hop = getHop();
	currentLocation += hop;

	if (currentLocation < 0)
	{
		currentLocation = 0;
	}

	printMessage(LogLevel::DEBUG, 
		[this, hop]() { return generateHopMessage(hop); });
}

void Rabbit::performWinnerActions()
{
	printMessage(LogLevel::INFO, "Completed race");

	{
		std::unique_lock<std::mutex> lock(contestParams.finishMutex);
		contestParams.winnerRabbitId = rabbitParams.id;
	}

	contestParams.cvFinish.notify_all();
}

void Rabbit::runRace()
{
	resetParams();

	while (currentLocation < contestParams.raceTarget)
	{
		takeNap();

		if (someoneElseWon)
		{
			printMessage(LogLevel::DEBUG, "Concedes");
			return;
		}

		makeHop();
	}

	performWinnerActions();
}

void Rabbit::resetParams()
{
	currentLocation = 0;
	someoneElseWon = false;
}

void Rabbit::printMessage(LogLevel intendedLogLevel, 
	std::string const& msg) const
{
	printMessage(intendedLogLevel, [&msg]() { return msg; });
}

void Rabbit::printMessage(LogLevel intendedLogLevel,
	std::function<std::string()> f) const
{
	::printMessage(
		rabbitParams.logLevel,
		intendedLogLevel,
		rabbitParams.printMutex,
		[&rabbitParams = rabbitParams, f]()
		{
			std::ostringstream oss;
			oss << "rabbit: " << std::right << std::setw(2)
				<< rabbitParams.id << ": " << f();
			return oss.str();
		});
}

RabbitParams const& Rabbit::getRabbitParams() const
{
	return rabbitParams;
}

int Rabbit::getCurrentLocation() const
{
	return currentLocation;
}
