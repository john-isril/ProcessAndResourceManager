#include "Process.h"

Process::Process()
	: processState( FREE ), parentIndex ( -1 ), priorityLevel(-1) {}

void Process::addChild(unsigned short childProcessIndex) {
	children.push_back(childProcessIndex);
}

Process::State Process::getState() const {
	return processState;
}

void Process::setState(State newState) {
	processState = newState;
}

void Process::setParent(short parentIndex) {
	this->parentIndex = parentIndex;
}

unsigned short Process::getParent() const {
	return parentIndex;
}

size_t Process::getNumOfChildren() const {
	return children.size();
}

void Process::removeChild(unsigned short childIndex) {
	children.remove(childIndex);
}

void Process::removeChildren() {
	children.clear();
}

bool Process::isChild(unsigned short c) {
	std::list<unsigned short>::const_iterator p = children.begin();
	for (p; p != children.end(); ++p) {
		if (*p == c) {
			return true;
		}
	}
	return false;
	//bool found = (std::find(children.begin(), children.end(), c) != children.end());
	//return found;
}

void Process::addResource(unsigned short r, unsigned short k) {
	resources.push_back(std::make_pair(r, k));
}

void Process::releaseResource(unsigned short r, unsigned short k) {
	std::list<std::pair<unsigned short, unsigned short>>::iterator it = resources.begin();

	for (it; it != resources.end(); ++it) {
		if (it->first == r) {
			if (it->second == k) {
				resources.remove(*it);
				return;
			}
			else {
				it->second -= k;
				return;
			}
		}
	}
}

void Process::releaseResources() {
	resources.clear();
}

void Process::setFree() {
	setState(Process::State::FREE);
	releaseResources();
	setParent(-1);
	removeChildren();
}

bool Process::hasParent() const {
	return (parentIndex != -1);
}

bool Process::hasResource(unsigned short r) const {
	std::list<std::pair<unsigned short, unsigned short>>::const_iterator p = resources.begin();
	for (p; p != resources.end(); ++p) {
		if (p->first == r) {
			return true;
		}
	}
	return false;
	//bool found = (std::find(children.begin(), children.end(), r) != children.end());
	//return found;
}

void Process::setPriorityLevel(short p) {
	priorityLevel = p;
}

short Process::getPriorityLevel() const {
	return priorityLevel;
}