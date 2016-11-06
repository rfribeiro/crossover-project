#include "CClientConfiguration.h"



CClientConfiguration::CClientConfiguration(int id, string key)
{
	m_id = id;
	m_key = key;
	m_alert_count = 0;
}

CClientConfiguration::~CClientConfiguration()
{
}

void CClientConfiguration::addAlert(CAlert alert)
{
	if (m_alert_count < MAX_ALERTS)
	{
		m_alerts[m_alert_count] = alert;
		m_alert_count++;
	}
}
