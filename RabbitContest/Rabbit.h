#pragma once

#include <mutex>
#include <chrono>
#include <random>

#include "RabbitContestParams.h"

class Rabbit
{
public:
	Rabbit(std::size_t rabbitId, RabbitContestParams& contestParams);
	void runRace();
private:
	void printMessage(std::string const& msg) const;
	int getHop();
	bool takeNap();

private:
	unsigned int id = 0;
	RabbitContestParams& contestParams;

	int currentLocation = 0;

	std::mt19937 randomGenerator;
	std::uniform_int_distribution<> hopRandomizer;
	std::uniform_int_distribution<> napRandomizer;
};
