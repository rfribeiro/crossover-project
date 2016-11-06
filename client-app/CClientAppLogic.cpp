#include "CClientAppLogic.h"
#include "CClientConnection.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using boost::property_tree::ptree;
using boost::property_tree::write_json;
using namespace std;

const string string_client_id = "id";
const string string_key = "key";
const string string_memory = "memory";
const string string_cpu = "cpu";
const string string_processes = "processes";
const string string_timestamp = "timestamp";

CClientAppLogic::CClientAppLogic()
{
	machine_data = Singleton<CMachineData>::Instance();
	m_server_config = CServerConfiguration();
}

CClientAppLogic::~CClientAppLogic()
{
}

const string CClientAppLogic::getData()
{
	ptree pt;
	pt.put(string_client_id, m_server_config.getClientId());
	pt.put(string_key, m_server_config.getKey());
	pt.put(string_timestamp, machine_data->getTimestamp());
	pt.put(string_memory, machine_data->getMemory());
	pt.put(string_cpu, machine_data->getCpuUsage());
	pt.put(string_processes, machine_data->getProcess());

	ostringstream buf;
	write_json(buf, pt, false);
	return buf.str();
}

void CClientAppLogic::run()
{
	try
	{
		// update machine data 
		machine_data->update();

		// update server and key configuration data 
		m_server_config.update();
	
		boost::asio::io_service io_service;
		CClientConnection c(io_service, m_server_config.getIp(), m_server_config.getPort(), getData());
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
}
