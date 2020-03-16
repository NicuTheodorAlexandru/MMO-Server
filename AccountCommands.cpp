#include "AccountCommands.h"

AccountCommands::AccountCommands()
{
	CommandManager::get_callbacks["login"] = &AccountCommands::Login;
}

AccountCommands::~AccountCommands()
{
}

void AccountCommands::Logout(sf::TcpSocket* socket, std::list<std::string> params)
{
	//SACommand cmd(&Database::conn, "update ")
}

void AccountCommands::Login(sf::TcpSocket* socket, std::list<std::string> params)
{
	std::string username = params.back();
	params.pop_back();
	std::string password = params.back();
	params.pop_back();
	std::string command_type = "post";
	std::string command = "login";
	//check correct user pass combo
	SACommand cmd(&Database::conn, "select id_session from accounts where username = :1 and password = :2");
	cmd << username.c_str() << password.c_str();
	cmd.Execute();
	sf::Uint64 sessionId;
	if (cmd.isResultSet())
	{
		sessionId = cmd[1];
	}
	else
	{
		sf::Packet packet;
		sf::Uint8 status = 1;
		packet << command_type << command << status;
		socket->setBlocking(false);
		socket->send(packet);
		return;
	}
	//check the user isn't already logged in
	if (sessionId != 0)
	{
		sf::Packet packet;
		sf::Uint8 status = 2;
		packet << command_type << command << status;
		socket->setBlocking(false);
		socket->send(packet);
		return;
	}
	//assign session id and send it to user
	sessionId = (int)(rand() % 100000000) + 1;
	SACommand cmd1(&Database::conn, "update from accounts where username=:1 and password=:2 set id_session=:3");
	cmd1 << username.c_str() << password.c_str() << sessionId;
	cmd1.Execute();
	sf::Packet packet;
	sf::Uint8 status = 0;
	packet << command_type << command << status;
	socket->setBlocking(false);
	socket->send(packet);
}