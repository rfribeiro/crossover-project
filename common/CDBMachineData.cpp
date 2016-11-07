#include "CDBMachineData.h"
#include <iostream>
#include "../common/sqlite3pp.h"

using namespace std;

enum DBIndex
{
	ID = 1,
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
	try {
			sqlite3pp::database db("./test.db");

			sqlite3pp::command cmd(db, "CREATE TABLE IF NOT EXISTS machine_data(id INTEGER, timestamp TEXT, memory REAL, cpu REAL, processes REAL)");
			cout << cmd.execute() << endl;
		}
		catch (std::exception& ex) {
			cout << ex.what() << endl;
			DBConnectionStatus::DISCONNECTED;
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

			sqlite3pp::command cmd(db, "INSERT INTO machine_data (id, timestamp, memory, cpu, processes) VALUES (?, ?, ?, ?, ?)");

			cout << cmd.bind(DBIndex::ID, std::to_string(data->getId()), sqlite3pp::copy) << endl;
			cout << cmd.bind(DBIndex::TIMESTAMP, data->getTimestamp(), sqlite3pp::copy) << endl;
			cout << cmd.bind(DBIndex::MEMORY, std::to_string(data->getMemory()), sqlite3pp::copy) << endl;
			cout << cmd.bind(DBIndex::CPU, std::to_string(data->getCpuUsage()), sqlite3pp::copy) << endl;
			cout << cmd.bind(DBIndex::PROCESSES, std::to_string(data->getProcess()), sqlite3pp::copy) << endl;

			cout << cmd.execute() << endl;

			xct.commit();
		}
	}
	catch (std::exception& ex) {
		cout << ex.what() << endl;
		return DBDataStatus::WRITE_NOK;
	}

	return DBDataStatus::WRITE_OK;
}
