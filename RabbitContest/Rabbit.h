#pragma once

#include <mutex>
#include <chrono>
#include <random>

class Rabbit
{
public:
	Rabbit
	(
		unsigned int rabbitId
		, int target
		, std::mutex& printMtx
	);
	void runRace();
private:
	void printMessage(std::string const& msg) const;
	int getHop();
	void takeNap();

private:
	std::mt19937 randomGenerator;
	std::uniform_int_distribution<> hopRandomizer;
	std::uniform_int_distribution<> napRandomizer;
	unsigned int id = 0;
	int raceTarget;
	std::mutex& printMutex;
	int currentLocation = 0;
};
