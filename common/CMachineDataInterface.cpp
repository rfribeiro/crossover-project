#include "CMachineDataInterface.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using boost::property_tree::ptree;
using boost::property_tree::write_json;

const char* memory_string = "memory";
const char* cpu_string = "cpu";
const char* processes_string = "processes";
const char* timestamp_string = "timestamp";

CMachineDataInterface::CMachineDataInterface()
{
	m_memory = 0;
	m_process = 0;
	m_cpu = 0;
}

void CMachineDataInterface::update()
{
	namespace pt = boost::posix_time;
	m_timestamp = pt::to_iso_string(pt::second_clock::local_time());
	updateMemoryInfo();
	updateCpuUsageInfo();
	updateProcessInfo();
}

double  CMachineDataInterface::getMemory()
{
	return m_memory;
}

double  CMachineDataInterface::getProcess()
{
	return m_process;
}

double  CMachineDataInterface::getCpuUsage()
{
	return m_cpu;
}

const string CMachineDataInterface::getJSON()
{
	ptree pt;
	pt.put(timestamp_string, m_timestamp);
	pt.put(memory_string, m_memory);
	pt.put(cpu_string, m_cpu);
	pt.put(processes_string, m_process);

	ostringstream buf;
	write_json(buf, pt, false);
	return buf.str();
}

void CMachineDataInterface::writeJSON(string data)
{
	ptree pt2;
	std::istringstream is(data);
	read_json(is, pt2);
	m_timestamp = pt2.get<string>(timestamp_string);
	m_memory = pt2.get<double>(memory_string);
	m_cpu = pt2.get<double>(cpu_string);
	m_process = pt2.get<double>(processes_string);
}
