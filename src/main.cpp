#include <iostream>
#include "CommandManager.h"

#include <iostream>
using namespace std;

int main()
{
    CommandManager manager;

    manager.initialize();

    std::string shortcut;
    std::string command;

    std::cout << "Enter shortcut: ";
    std::cin >> shortcut;
    std::cout << "Enter command: ";
    std::cin >> command;
    manager.addCommand(shortcut, command);

    return 0;
}