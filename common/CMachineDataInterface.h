#pragma once
#include <iostream>

using namespace std;

#ifndef DATA_STRINGS
#define DATA_STRINGS

#endif

class CMachineDataInterface
{
public:
	CMachineDataInterface();
	CMachineDataInterface(int id, string key);
	CMachineDataInterface(int id, string key, double memory, double cpu, double process);
	CMachineDataInterface(int id, string key, double memory, double cpu, double process, string timestamp);
	void update();
	double getMemory();
	double getProcess();
	double getCpuUsage();
	string getTimestamp();
	string getKey();
	int getId();
	void setKey(string new_key);
	void setId(int id);
	const string getJSON();
	void writeJSON(string data);
	virtual ~CMachineDataInterface() = 0 {};
protected:
	void updateTimestamp();
	virtual void updateProcessInfo() = 0;
	virtual void updateMemoryInfo() = 0;
	virtual void updateCpuUsageInfo() = 0;

	double m_memory = 0;
	double m_process = 0;
	double m_cpu = 0;
	string m_timestamp;
	int m_id;
	string m_key;
};

