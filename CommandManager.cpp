#include "CommandManager.h"

std::map<std::string, std::function<void(sf::TcpSocket*, std::list<std::string>)>> CommandManager::get_callbacks;
std::map<std::string, std::function<void(sf::TcpSocket*, std::list<std::string>)>> CommandManager::post_callbacks;

CommandManager::CommandManager()
{
	
}

CommandManager::~CommandManager()
{
}

void CommandManager::Route(sf::TcpSocket* socket, sf::Packet packet)
{
	std::string command_type;
	std::string command;
	packet >> command_type >> command;
	std::list<std::string> params;
	std::string param;
	while (!packet.endOfPacket())
	{
		packet >> param;
		params.push_back(param);
	}
	if (command_type == "get")
	{
		get_callbacks[command](socket, params);
	}
	else if (command_type == "post")
	{
		post_callbacks[command](socket, params);
	}
}