#include <iostream>
#include <map>
#include <functional>
#include <SFML/System.hpp>
#include <thread>
#include <chrono>
#include "SocketManager.h"
#include "Global.h"
#include "CommandManager.h"
#include "Database.h"

std::map<std::string, std::function<void()>> commands;
void HandleCommands();

void Quit()
{
	running = false;
}

int main()
{
	sf::String str("ha");
	//setup commands
	commands["quit"] = std::bind(&Quit);
	//setup classes
	Database db;
	SocketManager socketManager;
	CommandManager commandManager;
	//setup threads
	std::thread cmdThread(HandleCommands);
	std::thread socketManagerThread(&SocketManager::Start, &socketManager);
	//
	//cleanup
	while (running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	cmdThread.join();
	socketManagerThread.join();
	return 0;
}

void HandleCommands()
{
	while (running)
	{
		std::string cmd;
		std::cout << ">>";
		std::cin >> cmd;
		if (commands[cmd] != NULL)
		{
			commands[cmd]();
		}
	}
}

