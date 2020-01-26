#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Machine.h"

void userInterface(Machine* machine) {
	std::string command;
	while (command != "exit") {
		if (command == "/load") {
			std::string fileName;
			std::cin >> fileName;
			machine->load(fileName);
		} 
		else if (command == "/zero") {
			int x, y;
			std::cin >> x >> y;
			machine->zeroCells(x,y);
		}
		else if (command == "/set") {
			int x, y;
			std::cin >> x >> y;
			machine->set(x,y);
		}
		else if (command == "/copy") {
			int x, y, z;
			std::cin >> x >> y >> z;
			machine->copy(x,y,z);
		}
		else if (command == "/mem") {
			int x, y;
			std::cin >> x >> y;
			machine->print(x,y);
			std::cout << std::endl;
		}
		else if (command == "/add") {
			std::string fileToBeAdded;
			std::cin >> fileToBeAdded;
			machine->add(fileToBeAdded);
		}
		else if (command == "/run") {
			machine->run();
		}
		else if (command == "/quote") {
			std::string input;
			std::cin >> input;
			machine->addFromString(input, machine->getMaxIndexUsed(), machine->getCapsLockCommandsSize());
		}
		else if (command == "/code") {
			machine->printCommands();
		}
			std::cout << "Please input a command" << std::endl;
		std::cin >> command;
	}

	std::cout << "Bye" << std::endl;
}

int main() {
	Machine* unlimitedRegistersMachine = new Machine();
	userInterface(unlimitedRegistersMachine);
	delete unlimitedRegistersMachine;
	return 0;
}