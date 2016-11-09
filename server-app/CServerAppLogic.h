#pragma once
#include <iostream>
#include "../common/Singleton.h"
#include "CServerConnection.h"
#include "../common/CMachineData.h"
#include "CConcurrentQueue.h"
#include "CClientData.h"
#include <map>
#include <unordered_map>

using namespace std;
#define MAX_CLIENTS 101

class CServerAppLogic
{
public:
	CServerAppLogic();
	~CServerAppLogic();
	void run();
	void receivedPackage(string data);
	string getResponseData();
	static void processData();
	static void processRequests();

private:
	void loadClientConfigurations();
	CMachineData* mapHttpToObject(string data);
	void readAndSaveData();
	void writeDatabase(CMachineData* data);
	void checkAlerts(CMachineData* data);
	bool validateKey(CMachineData* data);
	bool sendEmail(CClientData client, string data);
	CConcurrentQueue<string> m_queue;
	CClientData m_clients[MAX_CLIENTS];
	//boost::unordered_map<std::string, int> m_map_clients;
	unordered_map<std::string, int> m_map_clients;
};

