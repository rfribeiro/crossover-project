#include "CClientAppLogic.h"
#include "CClientConnection.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>
#include <CLogger.h>

boost::asio::io_service io_service_timer;
boost::posix_time::seconds interval(5); // 1 second
boost::asio::deadline_timer timer_tick(io_service_timer, interval);

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
	m_server_config = CClientConfiguration();
	machine_data = Singleton<CMachineData>::Instance();
	machine_data->setId(m_server_config.getClientId());
	machine_data->setKey(m_server_config.getKey());
}

CClientAppLogic::~CClientAppLogic()
{
}

const string CClientAppLogic::getData()
{
	ptree pt;
	//pt.put(string_client_id, machine_data->getId());
	pt.put(string_key, machine_data->getKey());
	pt.put(string_timestamp, machine_data->getTimestamp());
	pt.put(string_memory, std::round(machine_data->getMemory()));
	pt.put(string_cpu, std::round(machine_data->getCpuUsage()));
	pt.put(string_processes, machine_data->getProcess());

	ostringstream buf;
	write_json(buf, pt, false);
	return buf.str();
}

void CClientAppLogic::tick(const boost::system::error_code&)
{
	try
	{
		CClientAppLogic* logic = Singleton<CClientAppLogic>::Instance();
		logic->run();

		//timer.expires_at(timer.expires_at() + logic->m_server_config.getTimer());
		timer_tick.expires_at(timer_tick.expires_at() + interval);
		timer_tick.async_wait(CClientAppLogic::tick);
	}
	catch (std::exception& e)
	{
		LOG_ERROR << "Exception: " << e.what();
	}
}

void CClientAppLogic::init()
{
	timer_tick.async_wait(&CClientAppLogic::tick);
	io_service_timer.run();
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
		LOG_ERROR << "Exception: " << e.what();
	}
}
