#pragma once
#include "CAlert.h"
#include "../common/CMachineData.h"
#define MAX_ALERTS 3

class CClientData
{
public:
	CClientData();
	CClientData(int id, string key, string email);
	~CClientData();
	void addAlert(CAlertType type, CAlert alert);
	void removeAlert(CAlertType type);
	CAlert getAlert(CAlertType type);

	int getId();
	string getKey();
	string getEmail();

	bool checkAlert(CAlertType type, double alert);
private:
	int m_id;
	string m_key;
	string m_email;
	CAlert m_alerts[3];
	int m_alert_count;
};

