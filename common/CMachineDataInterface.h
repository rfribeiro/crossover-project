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
	void update();
	double getMemory();
	double getProcess();
	double getCpuUsage();
	string getTimestamp();
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
};

