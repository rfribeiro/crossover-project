#pragma once
#include <Singleton.h>
#include <CMachineDataWindows.h>

#ifdef _WIN32
class CMachineData : public CMachineDataWindows
#else
class CComputerData : public CComputerDataLinux
#endif
{
public:
	CMachineData();
	CMachineData(int id, string key);
	CMachineData(int id, string key, double memory, double cpu, double process);
	CMachineData(int id, string key, double memory, double cpu, double process, string timestamp);
protected:
};

