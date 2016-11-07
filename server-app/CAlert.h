#pragma once
#include <iostream>

using namespace std;

enum CAlertType
{
	ALERT_UNDEFINED = -1,
	ALERT_MEMORY = 0,
	ALERT_CPU,
	ALERT_PROCESSES
};

class CAlert
{
public:
	CAlert();
	CAlert(CAlertType type, double value);
	CAlert(CAlertType type, string value);
	CAlert(string type, string value);
	~CAlert();

	CAlertType getType();
	double getValue();
private:
	CAlertType m_type;
	double m_value;
};

