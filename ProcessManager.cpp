#include "ProcessManager.h"

ProcessManager::ProcessManager()
	: numOfAllocatedProcesses(1) {
	//std::cout.open("std::cout.txt");
	for (size_t i = 0; i < M; i++) {
		PCB[i].setFree();
	}

	PCB[0].setState(Process::State::READY);
	RCB[0].setInventory(1);
	RCB[1].setInventory(1);
	RCB[2].setInventory(2);
	RCB[3].setInventory(3);

	for (size_t i = 0; i < N; i++) {
		RCB[i].setFree();
	}

	readyList[0].clear();
	readyList[1].clear();
	readyList[2].clear();
	readyList[0].push_front(0);
	numOfAllocatedProcesses = 1;
	currentRunning = 0;
}

void ProcessManager::init() {
	for (size_t i = 0; i < M; i++) {
		PCB[i].setFree();
	}

	PCB[0].setState(Process::State::READY);
	RCB[0].setInventory(1);
	RCB[1].setInventory(1);
	RCB[2].setInventory(2);
	RCB[3].setInventory(3);
	
	for (size_t i = 0; i < N; i++) {
		RCB[i].setFree();
	}
	
	readyList[0].clear();
	readyList[1].clear();
	readyList[2].clear();
	readyList[0].push_front(0);
	numOfAllocatedProcesses = 1;
	currentRunning = 0;
	if (started) {
		started = false;
	}
	else {
		std::cout << "\n";
	}
	
	scheduler();
}

void ProcessManager::create(unsigned short p) {
	if (p < 1 || p > 3) {
		std::cout << "-1";
		return;
	}

	if (numOfAllocatedProcesses < M) {
		for (size_t j = 0; j < M; j++) {
			if (PCB[j].getState() == Process::State::FREE) {
				PCB[j].setPriorityLevel(p);
				PCB[j].setState(Process::State::READY);
				PCB[currentRunning].addChild(j);
				PCB[j].setParent(currentRunning);
				readyList[p].push_back(j);

				numOfAllocatedProcesses++;
				break;
			}
		}

		scheduler();
		
	}
	else {
		std::cout << "-1";
	}
}

size_t ProcessManager::subDestroy(unsigned short j) {
	size_t counter = 0;

	if (PCB[j].hasParent()) {
		PCB[PCB[j].getParent()].removeChild(j);
	}

	readyList[PCB[j].getPriorityLevel()].remove(j);

	for (int i = 0; i < N; i++) {
		RCB[i].removeFromWaitingList(j);
	}

	std::list<std::pair<unsigned short, unsigned short>>::iterator it1 = PCB[j].resources.begin();

	for (it1; it1 != PCB[j].resources.end(); ++it1) {
		RCB[it1->first].addUnitCount(it1->second);

		std::pair<short, short> p = RCB[it1->first].dequeueWaitlist();
		while (p.first != -1) {
			readyList[PCB[p.first].getPriorityLevel()].push_back(p.first);
			PCB[p.first].setState(Process::State::READY);
			PCB[p.first].addResource(it1->first, p.second);
			p = RCB[it1->first].dequeueWaitlist();
		}
	}

	PCB[j].releaseResources();
	PCB[j].setState(Process::State::FREE);

	std::list<unsigned short> temp(PCB[j].children);
	std::list<unsigned short>::iterator it;

	for (it = temp.begin(); it != temp.end(); ++it) {
		counter++;
		counter += subDestroy(*it);
	}

	return counter;
}
void ProcessManager::destroy(unsigned short j) {
	unsigned short tempParent = PCB[j].getParent();

	while (tempParent != -1 && tempParent != currentRunning) {
		tempParent = PCB[tempParent].getParent();
	}

	if (tempParent != currentRunning && j != currentRunning) {
		std::cout << "-1";
	}
	else {
		size_t totalDestroyed = subDestroy(j);
		totalDestroyed++;
		numOfAllocatedProcesses -= totalDestroyed;
		scheduler();
	}
}

void ProcessManager::request(unsigned short r, unsigned short n) {
	if (currentRunning == 0 || r < 0 || r > (N - 1)) {
		std::cout << "-1";
		return;
	}

	if ((r == 0 || r == 1) && n != 1) {
		std::cout << "-1";
		return;
	}
	else if (r == 2 && n > 2) {
		std::cout << "-1";
		return;
	}
	else if (r == 3 && n > 3) {
		std::cout << "-1";
		return;
	}
	
	std::list<std::pair<unsigned short, unsigned short>>::iterator it = PCB[currentRunning].resources.begin();
	for (it; it != PCB[currentRunning].resources.end(); ++it) {
		if (it->first == r) {
			if ((n + it->second) > RCB[r].inventory) {
				std::cout << "-1";
				return;
			}
		}
	}

	if (RCB[r].is_free() && RCB[r].getUnitCount() >= n) {
		RCB[r].reduceUnitCount(n);
		PCB[currentRunning].addResource(r, n);
	}
	else {
		PCB[currentRunning].setState(Process::State::BLOCKED);
		RCB[r].addProcessToWaitlist(currentRunning, n);
		readyList[PCB[currentRunning].getPriorityLevel()].pop_front();
	}

	scheduler();
}

void ProcessManager::release(unsigned short r, unsigned short n) {
	if (r < 0 || r > 3) {
		std::cout << "-1";
		return;
	}

	std::list<std::pair<unsigned short, unsigned short>>::iterator it = PCB[currentRunning].resources.begin();
	for (it; it != PCB[currentRunning].resources.end(); ++it) {
		if (it->first == r) {
			if (n > it->second) {
				std::cout << "-1";
				return;
			}
		}
	}

	std::pair<unsigned short, unsigned short> j;
	if (!PCB[currentRunning].hasResource(r)) {
		std::cout << "-1";
		return;
	}

	PCB[currentRunning].releaseResource(r, n);
	RCB[r].addUnitCount(n);

	j = RCB[r].dequeueWaitlist();
	while (j.first != -1) {	
		readyList[PCB[j.first].getPriorityLevel()].push_back(j.first);
		PCB[j.first].setState(Process::State::READY);
		PCB[j.first].addResource(r, j.second);
		j = RCB[r].dequeueWaitlist();
	}

	scheduler();
}

void ProcessManager::scheduler() {
	for (int i = 0; i < 3; i++) {
		if (!readyList[i].empty()) {
			currentRunning = readyList[i].front();
		}
	}
	std::cout << currentRunning << " ";
}

void ProcessManager::timeout() {
	unsigned short head;
	int i = 0;

	for (i = 0; i < 3; i++) {
		if (readyList[i].empty()) {
			break;
		}
		else {
			head = readyList[i].front();
		}
	}
	i--;

	readyList[i].pop_front();
	readyList[i].push_back(head);
	scheduler();
}