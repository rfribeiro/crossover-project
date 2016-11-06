#include "CMachineDataWindows.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

CMachineDataWindows::CMachineDataWindows()
{
}

CMachineDataWindows::~CMachineDataWindows()
{
}

void CMachineDataWindows::updateProcessInfo()
{
	DWORD aProcesses[1024], cbNeeded;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		// if some error set number of process to -1
		m_process = -1;
	}

	// Calculate how many process identifiers.
	m_process = cbNeeded / sizeof(DWORD);
}

void CMachineDataWindows::updateMemoryInfo()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	m_memory = statex.dwMemoryLoad;
}

float CMachineDataWindows::CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
	static unsigned long long _previousTotalTicks = 0;
	static unsigned long long _previousIdleTicks = 0;

	unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
	unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;

	float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

	_previousTotalTicks = totalTicks;
	_previousIdleTicks = idleTicks;
	return ret;
}

static unsigned long long FileTimeToInt64(const FILETIME & ft) { return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime); }

// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.  Returns -1.0 on error.
void CMachineDataWindows::updateCpuUsageInfo()
{
	FILETIME idleTime, kernelTime, userTime;
	m_cpu = GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
	m_cpu *= 100;
}