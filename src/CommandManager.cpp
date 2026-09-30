#include "CommandManager.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fileSystem = std::filesystem;

std::string CommandManager::getCommandsPath()
{
	const char* userProfile = std::getenv("USERPROFILE");
	std::cout << userProfile << std::endl;

	if (!userProfile) return "";

	fileSystem::path path = fileSystem::path(userProfile) / "Documents" / "TerminalShortcuts" / "commands.ps1";

	return path.string();
}

std::string CommandManager::getPSProfilePath()
{
	FILE* pipe = _popen("powershell.exe -NoProfile -Command \"$PROFILE\"", "r");

	if (!pipe) return "";
	char buff[256];
	std::string profilePath;

	while (fgets(buff, sizeof(buff), pipe)) profilePath += buff;

	_pclose(pipe);

	while (!profilePath.empty() && (profilePath.back() == '\n' || profilePath.back() == '\r')) profilePath.pop_back();
	return profilePath;
}

void CommandManager::addCommand(const std::string& shortcut, const std::string& command)
{
	//check if already loads commands.ps1
	std::ifstream profile(getCommandsPath());

	std::string functionDec = "function " + shortcut + " {";

	std::string line;
	bool cmdFound = false;

	//REPLACE LATER. SHORTCUTS SHOULD BE ABLE TO BE REASSIGNED
	while (std::getline(profile, line))
	{
		if (line == functionDec)
		{
			cmdFound = true;
			std::cout << "Error: Existing command.\n";
			return;
		}
	}

	profile.close();

	std::ofstream profileOutput(getCommandsPath(), std::ios::app);

	profileOutput << "\n\n";
	profileOutput << "function " << shortcut << " {\n";
	profileOutput << "\t" << command << " $args\n";
	profileOutput << "}\n";
}




void CommandManager::initialize()
{
	std::string cmdsPath = getCommandsPath();
	std::string profilePath = getPSProfilePath();

	if (cmdsPath.empty() || profilePath.empty()) return;

	//create dir for commands.ps1
	fileSystem::path cmdsFile(cmdsPath);
	fileSystem::create_directories(cmdsFile.parent_path());

	if (!fileSystem::exists(cmdsFile))
	{
		std::ofstream file(cmdsPath);
		if (file.is_open())
		{
			file << "# ShortcutTerminal generated file\n";
			file.close();
		}
	}

	fileSystem::path profileFile(profilePath);
	fileSystem::create_directories(profileFile.parent_path());

	if (!fileSystem::exists(profilePath))
	{
		std::ofstream file(profilePath);
		file.close();
	}

	//check if already loads commands.ps1
	std::ifstream profile(profilePath);

	std::string line;
	bool loaded = false;

	while (std::getline(profile, line))
	{
		if (line.find(cmdsPath) != std::string::npos)
		{
			loaded = true;
			break;
		}
	}

	profile.close();

	if (!loaded)
	{
		std::ofstream profileOutput(profilePath, std::ios::app);

		profileOutput << "\n#ShortcutTerminal\n";
		profileOutput << ". \"" << cmdsPath << "\"\n";

		profileOutput.close();
	}
}