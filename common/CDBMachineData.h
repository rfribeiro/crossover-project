#pragma once
#include "CMachineData.h"

enum DBConnectionStatus
{
	CONNECTED = 1,
	DISCONNECTED
};

enum DBDataStatus
{
	WRITE_OK = 1,
	WRITE_NOK
};

/*
	Abstraction layer for Database connection
*/
class CDBMachineData
{
public:
	CDBMachineData();
	~CDBMachineData();
	
	DBConnectionStatus open();
	DBConnectionStatus close();
	DBDataStatus write(CMachineData* data);

private:
};

