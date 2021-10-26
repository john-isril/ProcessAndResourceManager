#include "Resource.h"

Resource::Resource()
	: isFree( true ) {}

bool Resource::is_free() const {
	return isFree;
}

void Resource::changeState() {
	if (isFree) {
		isFree = 0;
	}
	else {
		isFree = 1;
	}
}

void Resource::addProcessToWaitlist(unsigned short i, unsigned short k) {
	waitlist.push_back(std::make_pair(i, k));
}

bool Resource::waitlist_is_free() const {
	return waitlist.empty();
}

std::pair<unsigned short, unsigned short> Resource::dequeueWaitlist() {
	std::list<std::pair<unsigned short, unsigned short>>::iterator it = waitlist.begin();
	std::pair<unsigned short, unsigned short> j;
	for (it; it != waitlist.end(); ++it) {
		if (it->second <= state) {
			state -= it->second;
			 j = std::make_pair(it->first, it->second);
			waitlist.remove(*it);
			return j;
		}
	}

	return j;
	//unsigned short head = waitlist.front();
	//waitlist.pop_front();
}

void Resource::removeFromWaitingList(unsigned short i) {
	std::list<std::pair<unsigned short, unsigned short>>::iterator it = waitlist.begin();
	for (it; it != waitlist.end(); ++it) {
		if (it->first == i) {
			waitlist.remove(*it);
			return;
		}
	}
}

void Resource::setFree() {
	isFree = true;
	waitlist.clear();
}

void Resource::setInventory(unsigned short i) {
	inventory = i;
	state = i;
}

unsigned short Resource::getUnitCount() const {
	return state;
}

void Resource::addUnitCount(unsigned short k) {
	state += k;
}

void Resource::reduceUnitCount(unsigned short k) {
	state -= k;
}

