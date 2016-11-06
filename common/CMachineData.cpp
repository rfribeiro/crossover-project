#include "CMachineData.h"

CMachineData::CMachineData()
{

}

CMachineData::CMachineData(int id, string key)
#ifdef _WIN32
	: CMachineDataWindows(id, key)
#else
	: CMachineDataInterface(id, key)
#endif
{
}

CMachineData::CMachineData(int id, string key, double memory, double cpu, double process)
#ifdef _WIN32
	: CMachineDataWindows(id, key, memory, cpu, process)
#else
	: CMachineDataInterface(id, key, memory, cpu, process)
#endif
{

}

CMachineData::CMachineData(int id, string key, double memory, double cpu, double process, string timestamp)
#ifdef _WIN32
	: CMachineDataWindows(id, key, memory, cpu, process, timestamp)
#else
	: CMachineDataInterface(id, key, memory, cpu, process, timestamp)
#endif
{
	m_timestamp = timestamp;
}