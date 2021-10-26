#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <list>

class Resource {
private:
	bool isFree;

	//friend class ProcessManager;

public:
	Resource();
	unsigned short inventory, state;
	std::list<std::pair<unsigned short, unsigned short>> waitlist;
	bool is_free() const;
	void changeState();
	void addProcessToWaitlist(unsigned short i, unsigned short k);
	bool waitlist_is_free() const;
	std::pair<unsigned short, unsigned short> dequeueWaitlist();
	void removeFromWaitingList(unsigned short i);
	void setFree();
	void setInventory(unsigned short i);
	unsigned short getUnitCount() const;
	void addUnitCount(unsigned short k);
	void reduceUnitCount(unsigned short k);
};

#endif // !RESOURCE_H
