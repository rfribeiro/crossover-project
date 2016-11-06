#pragma once
#include <iostream>

using namespace std;

enum AlertType
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
	CAlert(AlertType type, double value);
	~CAlert();

	AlertType getType();
	double getValue();
private:
	AlertType m_type;
	double m_value;
};

