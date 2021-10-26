#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <iostream>
#include <list>
#include "Process.h"
#include "Resource.h"

class ProcessManager {
private:
	bool started = true;
	static const size_t M = 16;
	static const size_t N = 4;
	Process PCB[M];
	Resource RCB[N];
	std::list<unsigned short> readyList[3];
	size_t numOfAllocatedProcesses;
	size_t currentRunning;

	size_t subDestroy(unsigned short j);

public:
	ProcessManager();
	void init();
	void create(unsigned short p);
	void destroy(unsigned short j);
	void request(unsigned short r, unsigned short n);
	void release(unsigned short r, unsigned short n);
	void timeout();
	void scheduler();
};
#endif