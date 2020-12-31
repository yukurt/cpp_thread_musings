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
	RabbitContestParams& contestParams;
	std::vector<std::thread> threads;
};
