#include "Thread.h"


void Thread::startThread()
{
	mythread = std::thread(threadFunc,this);
}

