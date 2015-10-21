#pragma once

#include <mutex>

class Lock
{
	std::mutex m_mutex;

public:
	void lock() { m_mutex.lock(); }
	void unlock() { m_mutex.unlock(); }
};


