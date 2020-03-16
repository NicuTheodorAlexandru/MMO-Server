#include "SocketManager.h"

std::map<sf::TcpSocket*, sf::Uint64> sessions;

SocketManager::SocketManager()
{
}

SocketManager::~SocketManager()
{
	if (sockets.size() > 0)
	{
		for (std::list<sf::TcpSocket*>::iterator it = SocketManager::sockets.begin(); it != SocketManager::sockets.end(); it++)
		{
			delete* it;
		}
	}
}

void SocketManager::Start()
{
	int port = 80;
	if (listener.listen(port) != sf::Socket::Done)
	{
		std::cout << "SocketManager couldn't establish listener on port " << port << "!" << std::endl;
	}
	else
	{
		std::cout << "Started server on port " << listener.getLocalPort() << " and ip " << sf::IpAddress::getPublicAddress() << std::endl;
	}
	sf::TcpSocket* socket;
	std::thread listenThread(&SocketManager::Listen, this);
	while (running)
	{
		socket = new sf::TcpSocket();
		if (listener.accept(*socket) != sf::Socket::Done)
		{
			std::cout << "Couldn't accept incoming socket!" << std::endl;
			delete socket;
		}
		else
		{
			bool notConnected = true;
			//check that ip isn't already connected
			for (auto sock : sockets)
			{
				if (sock->getRemoteAddress().getPublicAddress() == socket->getRemoteAddress().getPublicAddress())
				{
					notConnected = false;
					break;
				}
			}
			//accept socket connection
			if (notConnected)
			{
				std::cout << "Socket connected on ip " << socket->getRemoteAddress().getPublicAddress() << std::endl;
				sf::Packet packet;
				sf::Uint8 conn = 1;
				packet << conn;
				socket->setBlocking(false);
				socket->send(packet);
				sockets.push_back(socket);
				sessions[socket] = 0;
				socketSelector.add(*socket);
			}
			else
			{
				sf::Packet packet;
				sf::Uint8 conn = 0;
				packet << conn;
				socket->setBlocking(false);
				socket->send(packet);
				std::cout << "Ip already connected " << socket->getRemoteAddress().getPublicAddress() << std::endl;
			}
		}
	}
	listenThread.join();
}

void SocketManager::Listen()
{
	sf::Packet packet;
	while (running)
	{
		if (socketSelector.wait(sf::seconds(60.f)))
		{
			for (auto i : sockets)
			{
				sf::TcpSocket* socket = i;
				if (socketSelector.isReady(*socket))
				{
					socket->receive(packet);
					if (packet.getDataSize() == 0)
					{
						continue;
					}
					std::string command;
					packet >> command;
					std::list<std::string> params;
					while (!packet.endOfPacket())
					{
						std::string s;
						packet >> s;
						params.assign(1, s);
					}
					CommandManager::Route(socket, packet);
				}
			}
		}
		else
		{

		}
	}
}