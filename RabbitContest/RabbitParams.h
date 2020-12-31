#pragma once

#include <chrono>

struct RabbitParams
{
	RabbitParams(unsigned int id_)
		: id(id_)
	{}
	unsigned int id = -1;
	std::chrono::duration<int, std::milli> shortestNapTime{ 500 };
	std::chrono::duration<int, std::milli> longestNapTime{ 5000 };
};