#include "CDBMachineData.h"



CDBMachineData::CDBMachineData()
{
}


CDBMachineData::~CDBMachineData()
{
}

DBConnectionStatus CDBMachineData::open()
{
	return DBConnectionStatus::DISCONNECTED;
}

DBConnectionStatus CDBMachineData::close()
{
	return DBConnectionStatus::DISCONNECTED;
}

DBDataStatus CDBMachineData::write(const CMachineData* data)
{
	return DBDataStatus::WRITE_NOK;
}
