#include "Database.h"
SAConnection Database::conn;

Database::Database()
{
	try
	{
		conn.Connect("mmo", "newuser", "parola", SA_MySQL_Client);
	}
	catch (SAException x)
	{
		conn.Rollback();
		std::cout << "Couldn't connect to database!" << std::endl;
	}
}

Database::~Database()
{
	conn.Disconnect();
}
