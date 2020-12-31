#pragma once

#include <random>
#include <functional>

#include "utils.h"

struct RabbitParams;
struct RabbitContestParams;

class Rabbit
{
public:
	Rabbit(RabbitParams& rabbitParams_, RabbitContestParams& contestParams_);
	void runRace();
	RabbitParams const& getRabbitParams() const;
	int getCurrentLocation() const;

private:
	void resetParams();
	std::string generateHopMessage(int hop) const;
	void makeHop();
	void performWinnerActions();
	void printMessage(LogLevel intendedLogLevel, std::string const& msg) const;
	void printMessage(LogLevel intendedLogLevel,
		std::function<std::string()> f) const;

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
