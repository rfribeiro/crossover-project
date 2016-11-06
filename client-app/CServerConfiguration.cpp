#include "CServerConfiguration.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


CServerConfiguration::CServerConfiguration()
{
}

CServerConfiguration::~CServerConfiguration()
{
	read();
}

void CServerConfiguration::update()
{
	read();
}

string CServerConfiguration::getIp()
{
	return m_ip;
}

string CServerConfiguration::getPort()
{
	return m_port;
}

string CServerConfiguration::getKey()
{
	return m_key;
}

int CServerConfiguration::getClientId()
{
	return m_client_id;
}

void CServerConfiguration::read()
{
	try {
		using boost::property_tree::ptree;
		ptree pt;

		read_xml(CONFIGURATION_FILE_NAME, pt);

		m_ip = pt.get<std::string>("ip");
		m_port = pt.get("port", 0);
		m_key = pt.get<std::string>("key");
	}
	catch (exception e)
	{
		// log
	}
}
