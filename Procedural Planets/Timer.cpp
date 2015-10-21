#include "Timer.h"

Timer::Timer()
	: m_start(0.0f)
{
}

Timer::~Timer()
{
}

void Timer::start()
{
	
	QueryPerformanceFrequency(&m_currentTime);
	freq = double(m_currentTime.QuadPart);

	QueryPerformanceCounter(&m_currentTime);
	timer = double(m_currentTime.QuadPart);

	m_start = timer;
}

double Timer::time()
{
    return double((getCurrentTime() - m_start)/freq);
}

float Timer::getCurrentTime() 
{
	QueryPerformanceCounter(&m_currentTime);
	return (float)m_currentTime.QuadPart;
}