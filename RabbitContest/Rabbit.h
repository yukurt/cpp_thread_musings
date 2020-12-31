#pragma once

#include <random>

#include "utils.h"

struct RabbitParams;
struct RabbitContestParams;

class Rabbit
{
public:
	Rabbit(RabbitParams& rabbitParams_, RabbitContestParams& contestParams_);
	void runRace();
private:
	void makeHop();
	void performWinnerActions();
	void printMessage(LogLevel intendedLogLevel, std::string const& msg) const;

	int getHop();
	void takeNap();

private:
	RabbitParams& rabbitParams;
	RabbitContestParams& contestParams;

	int currentLocation = 0;

	std::mt19937 randomGenerator;
	std::uniform_int_distribution<> hopRandomizer;
	std::uniform_int_distribution<> napRandomizer;

	bool someoneElseWon = false;
};
