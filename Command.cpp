#include "Command.h"
#include <string>


Command::Command()
{
}

Command::Command(std::string name, std::vector<std::string> arguments)
{
	this->name = name;
	this->arguments = arguments;
}

void Command::addParameters(std::stringstream& ss, int offset, int capsLockOffset)
{
	std::string currentCommandName = this->name;
	if (currentCommandName == "/zero") {
		std::string from;
		std::string to;
		ss >> from >> to;
		int newFrom = std::stoi(from) + offset;
		int newTo = std::stoi(to) + offset;
		this->arguments.push_back(std::to_string(newFrom));
		this->arguments.push_back(std::to_string(newTo));
	}
	else if (currentCommandName == "/set") {
		std::string current;
		std::string to;
		ss >> current >> to;
		int newCurrent = std::stoi(current) + offset;
		this->arguments.push_back(std::to_string(newCurrent));
		this->arguments.push_back(to);
	}
	else if (currentCommandName == "/copy") {
		std::string from;
		std::string to;
		std::string length;
		ss >> from >> to >> length;
		int newFrom = std::stoi(from) + offset;
		int newTo = std::stoi(to) + offset;
		int intLenght = std::stoi(length);
		this->arguments.push_back(std::to_string(newFrom));
		this->arguments.push_back(std::to_string(newTo));
		this->arguments.push_back(length);
	}
	else if (currentCommandName == "/mem") {
		std::string from;
		std::string to;
		ss >> from >> to;
		int newFrom = std::stoi(from) + offset;
		int newTo = std::stoi(to) + offset;
		this->arguments.push_back(std::to_string(newFrom));
		this->arguments.push_back(std::to_string(newTo));
	}
	else if (currentCommandName == "/add") {
		std::string fileName;
		ss >> fileName;
		this->arguments.push_back(fileName);
	}
	else if (currentCommandName == "/quote") {
		std::string args;
		getline(ss, args);
		this->arguments.push_back(args);
	}
	else if (currentCommandName == "ZERO") {
		std::string current;
		ss >> current;
		int newCurrent = std::stoi(current) + offset;
		this->arguments.push_back(std::to_string(newCurrent));
	}
	else if (currentCommandName == "INC") {
		std::string current;
		ss >> current;
		int newCurrent = std::stoi(current) + offset;
		this->arguments.push_back(std::to_string(newCurrent));
	}
	else if (currentCommandName == "MOVE") {
		std::string current;
		std::string to;
		ss >> current >> to;
		int newCurrent = std::stoi(current) + offset;
		this->arguments.push_back(std::to_string(newCurrent));
		int newTo = std::stoi(to) + offset;
		this->arguments.push_back(std::to_string(newTo));
	}
	else if (currentCommandName == "JUMP") {
		std::string firstParameter;
		ss >> firstParameter;

		std::string secondParameter;
		std::string thirdParameter;
		//used in order to obtain the other parameters correctly if they are present
		getline(ss, secondParameter, ' ');
		if (getline(ss, secondParameter,' ')) {
			ss >> thirdParameter;
		}

		if (secondParameter != "") {
			int newFirst = std::stoi(firstParameter) + offset;
			this->arguments.push_back(std::to_string(newFirst));
			int newSecond = std::stoi(secondParameter) + offset;
			this->arguments.push_back(std::to_string(newSecond));
			this->arguments.push_back(std::to_string(std::stoi(thirdParameter) + capsLockOffset));
		}
		else {
			this->arguments.push_back(std::to_string(std::stoi(firstParameter) + capsLockOffset));
		}
	}
	else {
		return;
	}
}


Command::~Command()
{
}
