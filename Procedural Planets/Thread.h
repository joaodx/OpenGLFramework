#pragma once

#include <thread>

class Thread
{

private:

	static void threadFunc(void* param) 
	{
		if(param)
			((Thread*) param)->run();
	};

	std::thread mythread;
	
	bool _terminate;

public:

	Thread(): _terminate(false){}
	virtual ~Thread(){}

	void terminate() { close(); _terminate = true; }
	bool isTermineted() const { return _terminate; }

	void startThread();
	void waitForTermination() 
	{
		if(_terminate)
		{
			//Sleep(1000);
			mythread.join();	
		}
	}

	void join() { mythread.join(); }
	
	virtual void run() = 0;
	virtual void close() { }

};


