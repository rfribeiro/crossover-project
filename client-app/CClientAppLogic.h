#include <iostream>
#include <boost/asio.hpp>
#include <Singleton.h>
#include <CMachineData.h>
#include "CClientConfiguration.h"

using namespace std;

class CClientAppLogic
{
public:
	CClientAppLogic();
	~CClientAppLogic();
	void run();
	void init();
	static void tick(const boost::system::error_code&);
private:
	const string getData();
	CMachineData* machine_data;
	CClientConfiguration m_server_config;
};
