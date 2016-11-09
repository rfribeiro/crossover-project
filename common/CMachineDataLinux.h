#pragma once
#include <CMachineDataInterface.h>

class CMachineDataLinux :
	public CMachineDataInterface
{
public:
	CMachineDataLinux();
	~CMachineDataLinux();
protected:
	void updateProcessInfo();
	void updateMemoryInfo();
	void updateCpuUsageInfo();
};

