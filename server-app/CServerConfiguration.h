#pragma once
#include <string>

using namespace std;

#define CONFIGURATION_FILE_NAME "server-configuration.xml"

class CServerConfiguration
{
public:
	CServerConfiguration();
	~CServerConfiguration();

	string getSmtpAddress();
	string getSmtpUserEmail();
	string getSmtpUserPassword();
	string getSmtpPort();
	int getServerPort();
	void update();

private:
	void loadData();

	int m_server_port = 9999;
	string m_smtp_address = "smtp.gmail.com";
	string m_smtp_port = "587";
	string m_smtp_email = "rfribeiro.crossover@gmail.com";
	string m_smtp_password = "!1qaz@2wsx";
};

