#pragma once
#include <SQLAPI.h>
#include <iostream>

class Database
{
public:
	Database();
	~Database();
	static SAConnection conn;
private:
};

