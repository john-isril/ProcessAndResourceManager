#include "Shell.h"

Shell::Shell() {}

void Shell::start() {
	ProcessManager systemProcessManager;

	std::string command;

	while (std::getline(std::cin, command)) {
		if (command[0] == 'c' && command[1] == 'r') {
			systemProcessManager.create(command[3] - '0');
		}
		else if (command[0] == 'd' && command[1] == 'e') {
			systemProcessManager.destroy(command[3] - '0');
		}
		else if (command[0] == 'r' && command[1] == 'q') {

			systemProcessManager.request(command[3] - '0', command[5] - '0');
		}
		else if (command[0] == 'r' && command[1] == 'l') {
			systemProcessManager.release(command[3] - '0', command[5] - '0');
		}
		else if (command[0] == 't' && command[1] == 'o') {
			systemProcessManager.timeout();
		}
		else if (command[0] == 'i' && command[1] == 'n') {
			systemProcessManager.init();
		}
	}
}