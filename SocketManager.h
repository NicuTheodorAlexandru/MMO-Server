#pragma once
#include <list>
#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include "Global.h"
#include <functional>
#include <thread>
#include "CommandManager.h"

class SocketManager
{
public:
	SocketManager();
	~SocketManager();
	void Start(), Listen(), Disconnect(sf::TcpSocket);
	std::map<sf::TcpSocket*, sf::Uint64> sessions;
private:
	std::list<sf::TcpSocket*> sockets;
	sf::TcpListener listener;
	sf::SocketSelector socketSelector;
};

