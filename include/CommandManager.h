#pragma once

#include <string>
#include <unordered_map>

class CommandManager
{
private:
	std::unordered_map<std::string, std::string> commands;

	std::string getCommandsPath();
	std::string getPSProfilePath();

public:
	void addCommand(const std::string& shortcut, const std::string& command);

	void initialize();
};