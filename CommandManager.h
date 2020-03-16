#pragma once
#include <SFML/Network.hpp>
#include "SocketManager.h"

class CommandManager
{
public:
	CommandManager();
	~CommandManager();
	static std::map<std::string, std::function<void(sf::TcpSocket*, std::list<std::string>)>> get_callbacks;
	static std::map<std::string, std::function<void(sf::TcpSocket*, std::list<std::string>)>> post_callbacks;
	static void Route(sf::TcpSocket* socket, sf::Packet packet);
};

