#include <iostream>
#include <iomanip>

#include "Rabbit.h"
#include "RabbitParams.h"
#include "RabbitContest.h"
#include "RabbitContestParams.h"

RabbitContest::RabbitContest(RabbitContestParams& contestParams_)
	: contestParams(contestParams_)
{
}

void RabbitContest::startRace()
{
	printMessage(LogLevel::INFO, "RACE BEGINS");
	
	resetParams();

	for (auto& rabbit : contestParams.rabbits)
	{
		threads.emplace_back(&Rabbit::runRace, &rabbit);
	}

	if (contestParams.enableRaceMonitoring)
	{
		threads.emplace_back(&RabbitContest::monitorRace, this);
	}

	for (auto& thr : threads)
	{
		if (thr.joinable())
		{
			thr.join();
		}
	}

	printMessage(LogLevel::INFO, "RACE ENDS");
}

void RabbitContest::resetParams()
{
	someoneWon = false;
	threads.clear();
	contestParams.winnerRabbitId = -1;
}

void RabbitContest::waitMonitorInterval()
{
	std::unique_lock<std::mutex> lock(contestParams.finishMutex);
	someoneWon = contestParams.cvFinish.wait_for(lock,
		std::chrono::milliseconds(contestParams.monitorInterval),
		[&contestParams = contestParams]()
		{
			return contestParams.winnerRabbitId >= 0;
		});
}

void RabbitContest::monitorRace()
{
	printRaceStatus();
	while (true)
	{
		waitMonitorInterval();
		printRaceStatus();

		if (someoneWon)
		{
			return;
		}
	}
}

void RabbitContest::printRaceStatus()
{
	std::lock_guard<std::mutex> lock(contestParams.printMutex);

	std::cout << getCurrentTimestamp() << ": Race status:\n";
	printStatusBorder();
	for (auto const& rabbit : contestParams.rabbits)
	{
		printRabbitLocation(rabbit);
	}
	printStatusBorder();
}

void RabbitContest::printRabbitLocation(const Rabbit& rabbit)
{
	auto rabbitId = rabbit.getRabbitParams().id;
	auto location = rabbit.getCurrentLocation();

	std::cout << std::setw(2) << rabbitId << ": ";

	if (location > contestParams.raceTarget)
	{
		for (int i = 0; i <= contestParams.raceTarget - 1; ++i)
		{
			std::cout << "-";
		}
		std::cout << "+";
	}
	else
	{
		for (int i = 0; i < location; ++i)
		{
			std::cout << "-";
		}

		std::cout << "+";

		for (int i = location + 1; i <= contestParams.raceTarget; ++i)
		{
			std::cout << " ";
		}
	}

	std::cout << "|" << std::endl;
}

void RabbitContest::printStatusBorder()
{
	for (int i = 0; i < 6 + contestParams.raceTarget; ++i)
	{
		std::cout << "*";
	}
	std::cout << std::endl;
}

void RabbitContest::printMessage(LogLevel intendedLogLevel,
	std::string const& msg) const
{
	::printMessage(
		contestParams.logLevel,
		intendedLogLevel,
		contestParams.printMutex,
		[&msg]()
		{
			return "contest: " + msg;
		});
}