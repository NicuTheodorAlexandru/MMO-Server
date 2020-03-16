#pragma once
#include <SQLAPI.h>
#include <iostream>
#include "Database.h"
#include <SFML/Network.hpp>
#include <list>
#include "CommandManager.h"

class AccountCommands
{
public:
	AccountCommands();
	~AccountCommands();
	static void Login(sf::TcpSocket*, std::list<std::string> params);
	static void Logout(sf::TcpSocket*, std::list<std::string> params);
};

