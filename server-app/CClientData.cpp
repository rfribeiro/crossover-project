#include "CClientData.h"



CClientData::CClientData()
{
}

CClientData::CClientData(int id, string key, string email)
{
	m_id = id;
	m_key = key;
	m_email = email;
}

CClientData::~CClientData()
{
}

void CClientData::addAlert(CAlertType type, CAlert alert)
{
	if (type < MAX_ALERTS)
	{
		m_alerts[type] = alert;
	}
}

void CClientData::removeAlert(CAlertType type)
{

}

CAlert CClientData::getAlert(CAlertType type)
{
	return m_alerts[type];
}

void CClientData::setId(int id)
{
	m_id = id;
}

void CClientData::setKey(string key)
{
	m_key = key;
}

void CClientData::setEmail(string mail)
{
	m_email = mail;
}

int CClientData::getId()
{
	return m_id;
}

string CClientData::getKey()
{
	return m_key;
}

string CClientData::getEmail()
{
	return m_email;
}

bool CClientData::checkAlert(CAlertType type, double value)
{
	if (m_alerts[type].getValue() > 0 && m_alerts[type].getValue() < value)
	{
		return true;
	}
	return false;
}
