#pragma once
#include "CMachineDataInterface.h"

class CMachineDataWindows : public CMachineDataInterface
{
public:
	CMachineDataWindows();
	~CMachineDataWindows();
protected:
	void updateProcessInfo();
	void updateMemoryInfo();
	void updateCpuUsageInfo();
private:
	float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
};

