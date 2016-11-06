#pragma once
#include "CMachineDataInterface.h"

class CMachineDataWindows : public CMachineDataInterface
{
public:
	CMachineDataWindows();
	~CMachineDataWindows();
	CMachineDataWindows(int id, string key) : CMachineDataInterface(id, key) {};
	CMachineDataWindows(int id, string key, double memory, double cpu, double process)
		: CMachineDataInterface(id, key, memory, cpu, process) {};
	CMachineDataWindows(int id, string key, double memory, double cpu, double process, string timestamp)
		: CMachineDataInterface(id, key, memory, cpu, process, timestamp) {};
protected:
	void updateProcessInfo();
	void updateMemoryInfo();
	void updateCpuUsageInfo();
private:
	float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
};

