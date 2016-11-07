#include "CClientConfiguration.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


CClientConfiguration::CClientConfiguration()
{
	read();
}

CClientConfiguration::~CClientConfiguration()
{
	
}

void CClientConfiguration::update()
{
	read();
}

string CClientConfiguration::getIp()
{
	return m_ip;
}

string CClientConfiguration::getPort()
{
	return m_port;
}

string CClientConfiguration::getKey()
{
	return m_key;
}

int CClientConfiguration::getClientId()
{
	return m_client_id;
}

int CClientConfiguration::getTimer()
{
	return m_timer;
}

void CClientConfiguration::read()
{
	try {
		using boost::property_tree::ptree;
		ptree pt;

		read_xml(CONFIGURATION_FILE_NAME, pt);

		m_ip = pt.get<std::string>("configuration.address");
		m_port = pt.get<std::string>("configuration.port");
		m_key = pt.get<std::string>("configuration.key");
		m_client_id = pt.get<int>("configuration.id");
		m_timer = pt.get<int>("configuration.timer");
	}
	catch (exception e)
	{
		// log
	}
}
