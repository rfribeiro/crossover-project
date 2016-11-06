#pragma once
#include <iostream>
#include "../common/Singleton.h"
#include "CServerConnection.h"

using namespace std;

class CServerAppLogic
{
public:
	CServerAppLogic();
	~CServerAppLogic();
	void run();
	void receivedPackage(session* s, string data, size_t bytes_transferred);
private:
	string getResponseData();
	void loadClientConfigurations();
	void writeDatabase(string data);
	void sendPackage();
};

