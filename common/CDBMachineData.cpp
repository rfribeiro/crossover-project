#include "CDBMachineData.h"
#include <iostream>
#include "../common/sqlite3pp.h"
#include <CLogger.h>

using namespace std;

enum DBIndex
{
	//ID = 1,
	KEY = 1,
	TIMESTAMP,
	MEMORY,
	CPU,
	PROCESSES
};


CDBMachineData::CDBMachineData()
{

}


CDBMachineData::~CDBMachineData()
{
}

DBConnectionStatus CDBMachineData::open()
{
	try 
	{
		sqlite3pp::database db("./test.db");

		sqlite3pp::command cmd(db, "CREATE TABLE IF NOT EXISTS machine_data(key TEXT, timestamp TEXT, memory REAL, cpu REAL, processes REAL, PRIMARY KEY (key, timestamp))");
		cmd.execute();
	}
	catch (std::exception& ex) {
		LOG_ERROR << ex.what();
		return DBConnectionStatus::DISCONNECTED;
	}

	return DBConnectionStatus::CONNECTED;
}

DBConnectionStatus CDBMachineData::close()
{
	return DBConnectionStatus::DISCONNECTED;
}

DBDataStatus CDBMachineData::write(CMachineData* data)
{
	try {
		sqlite3pp::database db("test.db");

		{

			sqlite3pp::transaction xct(db);

			sqlite3pp::command cmd(db, "INSERT INTO machine_data (key, timestamp, memory, cpu, processes) VALUES (?, ?, ?, ?, ?)");

			//cmd.bind(DBIndex::ID, std::to_string(data->getId()), sqlite3pp::copy);
			cmd.bind(DBIndex::KEY, data->getKey(), sqlite3pp::copy);
			cmd.bind(DBIndex::TIMESTAMP, data->getTimestamp(), sqlite3pp::copy);
			cmd.bind(DBIndex::MEMORY, std::to_string(data->getMemory()), sqlite3pp::copy);
			cmd.bind(DBIndex::CPU, std::to_string(data->getCpuUsage()), sqlite3pp::copy);
			cmd.bind(DBIndex::PROCESSES, std::to_string(data->getProcess()), sqlite3pp::copy);

			cmd.execute();

			xct.commit();
		}
	}
	catch (std::exception& ex) {
		LOG_ERROR << ex.what();
		return DBDataStatus::WRITE_NOK;
	}

	return DBDataStatus::WRITE_OK;
}
