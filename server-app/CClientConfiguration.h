#pragma once
#include "CAlert.h"

#define MAX_ALERTS 3

class CClientConfiguration
{
public:
	CClientConfiguration(int id, string key);
	~CClientConfiguration();
	void addAlert(CAlert alert);
private:
	int m_id;
	string m_key;
	CAlert m_alerts[3];
	int m_alert_count;
};

