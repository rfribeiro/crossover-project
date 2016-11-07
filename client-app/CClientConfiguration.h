#pragma once
#include <iostream>

using namespace std;

#define CONFIGURATION_FILE_NAME "client-configuration.xml"

class CClientConfiguration
{
public:
	CClientConfiguration();
	~CClientConfiguration();
	void update();
	string getIp();
	string getPort();
	string getKey();
	int getClientId();
	int getTimer();
protected:
	void read();
private:
	string m_ip = "localhost";
	string m_port = "9999";
	string m_key = "xxxx";
	int m_client_id = 1;
	int m_timer = 5;
};

