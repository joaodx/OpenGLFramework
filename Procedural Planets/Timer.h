#pragma once

#include <Windows.h>


class Timer
{
private:

	double m_start;
	LARGE_INTEGER m_currentTime;
	double timer;
	double freq;

public:

	Timer();
	~Timer();

	void start();
	double time();
	float getCurrentTime();
};
