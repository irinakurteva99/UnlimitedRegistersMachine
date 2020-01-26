#include "Machine.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

void Machine::load(std::string fileName)
{
	allCommands.clear();
	add(fileName);
}

void Machine::add(std::string fileName)
{
	std::string input;
	std::fstream in(fileName);
	int offset = this->maxIndexUsed;
	int capsLockOffset = capsLockCommands.size();
	if (in.is_open()) {
		while (getline(in, input)) {
			addFromString(input, offset, capsLockOffset);
		}
		in.close();
	}
	else {
		std::ofstream out(fileName);
		out.close();
	}
}

void Machine::addFromString(std::string input, int offset, int capsLockOffset)
{
	std::stringstream ss(input);
	std::string currentWord;
	while (getline(ss, currentWord, ' ')) {
		Command command;
		if (currentWord == "/comment") {
			// used to ignore the comments
			getline(ss, currentWord);
		}
		else {
			command.name = currentWord;
			command.addParameters(ss, offset, capsLockOffset);
			allCommands.emplace_back(command);
			if (command.name == "JUMP" || command.name == "ZERO" ||
				command.name == "INC" || command.name == "MOVE") {
				auto lastElement = allCommands.begin();
				advance(lastElement, allCommands.size() - 1);
				capsLockCommands.push_back(lastElement);
			}
		}
		// I was confused which commands exactly cound towards the memory range because the example uses
		// copy with the sixth cell in the main program but the range id [0..3]
		// more commands can always be added here if needed
		if (command.name == "ZERO" || command.name == "INC") {
			if (std::stoi(command.arguments[0]) + 1> maxIndexUsed) {
				maxIndexUsed = std::stoi(command.arguments[0]) + 1;
			}
		}
		else if (command.name == "/zero") {
			if (std::stoi(command.arguments[0]) > maxIndexUsed) {
				maxIndexUsed = std::stoi(command.arguments[0]) + 1;
			} else if (std::stoi(command.arguments[1]) > maxIndexUsed) {
				maxIndexUsed = std::stoi(command.arguments[1]) + 1;
			}
		}
	}
}

//in a lot of places there is a check if something is != memory.end()
//that is because empty cells in the memory (those with value of 0) are not representated in the
//unordered map
void Machine::zeroCells(int beginIndex, int endIndex) {
	for (int i = 0; i <= endIndex - beginIndex; i++) {
		auto currentElement = memory.find(beginIndex + i);
		if (currentElement != memory.end()) {
			memory.erase(currentElement);
		}
	}
}

void Machine::increaseCell(int index)
{
	if (memory.find(index) != memory.end()) {
		memory.at(index)++;
	}
	else {
		memory.emplace(index, 1);
	}
}

void Machine::set(int x, int y)
{
	if (memory.find(x) != memory.end()) {
		zeroCells(x, x);
	}
	memory.emplace(x, y);
}

void Machine::move(int x, int y)
{
	if (memory.find(x) != memory.end()) {
		if (memory.find(y) != memory.end()) {
			zeroCells(y, y);
		}
		memory.emplace(y, memory.at(x));
	}
	else {
		if (memory.find(y) != memory.end()) {
			zeroCells(y, y);
		}
	}
}

void Machine::copy(int begin, int beginNew, int length)
{
	//two cases in order to work correctly for ovelaping intervals
	if (beginNew > begin) {
		for (int i = length - 1; i >= 0; i--) {
			move(begin + i, beginNew + i);
		}
	}
	else {
		for (int i = 0; i < length; i++) {
			move(begin + i, beginNew + i);
		}
	}
}

void Machine::print(int beginIndex, int endIndex)
{
	for (int i = 0; i <= endIndex - beginIndex; i++) {
		if (memory.find(beginIndex + i) != memory.end()) {
			std::cout << memory.at(beginIndex + i) << " ";
		}
		else {
			std::cout << "0 ";
		}
	}
}

void Machine::run()
{
	auto command = allCommands.begin();
	while (command != allCommands.end()) {
		std::string currentCommandName = (*command).name;
		if (currentCommandName == "/zero") {
			int firstParameter = std::stoi((*command).arguments.at(0));
			int secondParameter = std::stoi((*command).arguments.at(1));
			this->zeroCells(firstParameter, secondParameter);
		} else if (currentCommandName == "/set") {
			this->set(std::stoi((*command).arguments.at(0)), std::stoi((*command).arguments.at(1)));
		} else if (currentCommandName == "/copy") {
			this->copy(std::stoi((*command).arguments.at(0)), std::stoi((*command).arguments.at(1)), std::stoi((*command).arguments.at(2)));
		} else if (currentCommandName == "/mem") {
			this->print(std::stoi((*command).arguments.at(0)), std::stoi((*command).arguments.at(1)));
		} else if (currentCommandName == "/add") {
			this->add((*command).arguments[0]);
		} else if (currentCommandName == "/quote") {
			this->addFromString((*command).arguments[0], maxIndexUsed, 0);
		} else if (currentCommandName == "ZERO") {
			int parameter = std::stoi((*command).arguments.at(0));
			this->zeroCells(parameter, parameter);
		} else if (currentCommandName == "INC") {
			int parameter = std::stoi((*command).arguments.at(0));
			increaseCell(parameter);
		} else if (currentCommandName == "MOVE") {
			this->move(std::stoi((*command).arguments.at(0)), std::stoi((*command).arguments.at(1)));
		} else if (currentCommandName == "JUMP") {
			if ((*command).arguments.size() == 3) {
				int x = std::stoi((*command).arguments.at(0));
				int y = std::stoi((*command).arguments.at(1));
				if (memory.find(x) == memory.end() && memory.find(y) == memory.end()) {
					int indexToJump = std::stoi((*command).arguments.at(2));
					if (indexToJump > 0 && indexToJump < capsLockCommands.size()) {
						command = capsLockCommands.at(indexToJump);
						//this continue and the ones below are used so that we don't increment the iterator one more time than neede
						//indexToJump - 1 doesn't work because the command iterator is over all comands
						//so when we increase it later on we might get a command that otherwise gets skipped
						continue;
					}
					else return;
				}
				else if (memory.find(x) != memory.end() && memory.find(y) != memory.end()) {
					if (memory.at(x) == memory.at(y)) {
						int indexToJump = std::stoi((*command).arguments.at(2));
						if (indexToJump > 0 && indexToJump < capsLockCommands.size()) {
							command = capsLockCommands.at(indexToJump);
							continue;
						}
						else return;
					}
				}
			}
			else {
				int indexToJump = std::stoi((*command).arguments.at(0));
					if (indexToJump > 0 && indexToJump < capsLockCommands.size()) {
					command = capsLockCommands.at(indexToJump);
					continue;
				}
				else return;
			}
		} else return;
		command++;
	}
}

void Machine::printCommands()
{
	for (Command c : allCommands) {
		std::cout << c.name << " ";
		for (std::string s : c.arguments) {
			std::cout << s << " ";
		}
		std::cout << std::endl;
	}
}

Machine::Machine()
{
}


Machine::~Machine()
{
}
