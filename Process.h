#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <list>

class Process {
public:
	enum State { FREE = -1, BLOCKED = 0, READY  = 1 };

private:
	State processState;
	short parentIndex;
	short priorityLevel;
	

	//friend class ProcessManager;

public:
	Process();
	std::list<std::pair<unsigned short, unsigned short>> resources;
	std::list<unsigned short> children;
	void addChild(unsigned short childProcessIndex);
	State getState() const;
	void setState(State newState);
	void setParent(short parentIndex);
	short getParent() const;
	size_t getNumOfChildren() const;
	void removeChild(unsigned short childIndex);
	void removeChildren();
	bool isChild(unsigned short c);
	void addResource(unsigned short r, unsigned short k);
	void releaseResource(unsigned short r, unsigned short k);
	void releaseResources();
	void setFree();
	bool hasParent() const;
	bool hasResource(unsigned short r) const;
	void setPriorityLevel(short p);
	short getPriorityLevel() const;
};

#endif