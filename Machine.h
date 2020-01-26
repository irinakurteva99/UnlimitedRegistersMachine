#pragma once
#include "Command.h"
#include <list>
#include <unordered_map>
#include <iostream>

///the main class which represents the Unlimited Registers Machine
class Machine
{
	///the memory of the machine, sparse array
	std::unordered_map<int, int> memory;
	///list of all commands, both the ones beginning with / and the ones in capsLock
	std::list<Command> allCommands;
	///vector with iterators to all the capsLock commands, used for jumping
	std::vector<std::list<Command>::iterator> capsLockCommands;
	///the max index of the memory used, used when adding new files in order to determine their memory indexes
	int maxIndexUsed = 0;
public:
	///the function for /load FileName
	void load(std::string fileName);
	///the function for /add FileName
	void add(std::string filName);
	///used for adding commands from string, extacted from file or /quote command, the two ints are the memory and command offsets
	void addFromString(std::string input, int, int);
	///used to zero the cells from index to index, if only one cell should be used, the two indexes are just the same
	void zeroCells(int beginIndex, int endIndex);
	///used for increasing the value in a cell
	void increaseCell(int index);
	///used to set the value in cell number x to y
	void set(int x, int y);
	///move the value in cell y to cell x
	void move(int x, int y);
	///the function for /copy
	void copy(int begin, int beginNew, int length);
	///used for printing some part of the memory to the console, in /mem for example
	void print(int beginIndex, int endIndex);
	///function for /run, executes all the commands loaded
	void run();
	///function for /code, prints all loaded commands with their arguments to the console
	void printCommands();
	int getMaxIndexUsed() { return maxIndexUsed; }
	int getCapsLockCommandsSize() { return capsLockCommands.size(); }
	Machine();
	~Machine();
};

