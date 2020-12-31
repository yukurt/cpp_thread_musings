#pragma once

#include <vector>
#include <thread>

struct RabbitContestParams;

class RabbitContest
{
public:
	RabbitContest(RabbitContestParams& contestParams_);
	void startRace();

private:
	void resetParams();
	void waitMonitorInterval();
	void monitorRace();
	void printRaceStatus();
	void printRabbitLocation(const Rabbit& rabbit);
	void printMessage(LogLevel intendedLogLevel,
		std::string const& msg) const;

private:
	RabbitContestParams& contestParams;
	std::vector<std::thread> threads;
	bool someoneWon = false;
};
