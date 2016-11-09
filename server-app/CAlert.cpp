#include "CAlert.h"
#include <boost/lexical_cast.hpp>
#include <CLogger.h>

CAlert::CAlert()
{
	m_type = ALERT_UNDEFINED;
	m_value = 0;
}

CAlert::CAlert(CAlertType type, double value)
{
	m_type = type;
	m_value = value;
}

CAlert::CAlert(CAlertType type, string value)
{
	try
	{
		m_type = type;
		if (value[value.length() - 1] == '%%')
		{
			value = value.substr(0, value.length() - 2);
		}
		m_value = boost::lexical_cast<double>(value);
	}
	catch (boost::bad_lexical_cast const& l)
	{
		m_value = 0;
		LOG_ERROR << l.what();
	}
	catch (std::exception& e)
	{
		LOG_ERROR << e.what();
	}
}

CAlert::CAlert(string type, string value)
{
	try
	{
		if (type == "memory")
			m_type = CAlertType::ALERT_MEMORY;
		else if (type == "cpu")
			m_type = CAlertType::ALERT_CPU;
		else if (type == "processes")
			m_type = CAlertType::ALERT_PROCESSES;
		else
			LOG_ERROR << "Alert type not valid";

		if (value[value.length() - 1] == '%')
		{
			value = value.substr(0, value.length() - 1);
		}
		m_value = boost::lexical_cast<double>(value);
	}
	catch (boost::bad_lexical_cast const& l)
	{
		m_value = 0;
		LOG_ERROR << l.what();
	}
	catch (std::exception& e)
	{
		LOG_ERROR << e.what();
	}
}

CAlert::~CAlert()
{
}

CAlertType CAlert::getType()
{
	return m_type;
}

double CAlert::getValue()
{
	return m_value;
}
