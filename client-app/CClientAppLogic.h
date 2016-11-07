#include <iostream>
#include "../common/Singleton.h"
#include "../common/CMachineData.h"
#include "CClientConfiguration.h"

using namespace std;

class CClientAppLogic
{
public:
	CClientAppLogic();
	~CClientAppLogic();
	void run();
private:
	const string getData();
	CMachineData* machine_data;
	CClientConfiguration m_server_config;
};
