#pragma once
#include <string>
#include <vector>
#include <sstream>

///the class which represents an instruction and its arguments
struct Command
{
	///the name of the instruction
	std::string name;
	///list of the parametes as strings so as to be the same format for all commands (/add and /quote need strings as their arguments)
	std::vector<std::string> arguments;
public:
	Command();
	Command(std::string name, std::vector<std::string> arguments);
	///used for adding parameters to an already created empty command
	void addParameters(std::stringstream&, int offset, int capsLockOffset);
	~Command();
};

