#pragma once
#include <iostream>

using namespace std;

#define CONFIGURATION_FILE_NAME "./server_configuration.xml"

class CServerConfiguration
{
public:
	CServerConfiguration();
	~CServerConfiguration();
	void update();
	string getIp();
	string getPort();
	string getKey();
	int getClientId();

protected:
	void read();
private:
	string m_ip = "localhost";
	string m_port = "9999";
	string m_key = "xxx";
	int m_client_id = 1;
};

