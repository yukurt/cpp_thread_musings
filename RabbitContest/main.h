#pragma once

void startRaces(RabbitContestParams& contestParams, const int& numRaces);

void createRabbits(std::vector<RabbitParams>& rabbitParamsCollection,
	std::vector<Rabbit>& rabbits, RabbitContestParams& contestParams);

std::vector<RabbitParams> createRabbitParamsCollection
	(int numRabbits, std::mutex& printMutex);
