#pragma once
#include "Singleton.h"
#include "CMachineDataWindows.h"

#ifdef _WIN32
class CMachineData : public CMachineDataWindows
#else
class CComputerData : public CComputerDataLinux
#endif
{
public:
	CMachineData();
protected:
	int m_memory = 0;
	int m_process = 0;
	int m_cpu = 0;
};

