#include "CAlert.h"

CAlert::CAlert()
{
	m_type = ALERT_UNDEFINED;
	m_value = 0;
}

CAlert::CAlert(AlertType type, double value)
{
	m_type = type;
	m_value = value;
}

CAlert::~CAlert()
{
}

AlertType CAlert::getType()
{
	return m_type;
}

double CAlert::getValue()
{
	return m_value;
}
