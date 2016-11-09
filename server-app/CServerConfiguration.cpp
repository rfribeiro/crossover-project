#include "CServerConfiguration.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <CLogger.h>

CServerConfiguration::CServerConfiguration()
{
	loadData();
}


CServerConfiguration::~CServerConfiguration()
{
}

string CServerConfiguration::getSmtpAddress()
{
	return m_smtp_address;
}

string CServerConfiguration::getSmtpUserEmail()
{
	return m_smtp_email;
}

string CServerConfiguration::getSmtpUserPassword()
{
	return m_smtp_password;
}

string CServerConfiguration::getSmtpPort()
{
	return m_smtp_port;
}

int CServerConfiguration::getServerPort()
{
	return m_server_port;
}

void CServerConfiguration::update()
{
	loadData();
}

void CServerConfiguration::loadData()
{
	try {
		using boost::property_tree::ptree;
		ptree pt;

		read_xml(CONFIGURATION_FILE_NAME, pt);

		m_server_port = pt.get<int>("configuration.port", 9999);
		m_smtp_address = pt.get<std::string>("configuration.smtp.server");
		m_smtp_port = pt.get<std::string>("configuration.smtp.port");
		m_smtp_email = pt.get<std::string>("configuration.smtp.user");
		m_smtp_password = pt.get<std::string>("configuration.smtp.password");
	}
	catch (exception e)
	{
		LOG_ERROR << e.what();
	}
}
