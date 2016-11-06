#include "CClientAppLogic.h"
#include "CClientConnection.h"

CClientAppLogic::CClientAppLogic()
{
	machine_data = Singleton<CMachineData>::Instance();
	m_server_config = CServerConfiguration();
}

CClientAppLogic::~CClientAppLogic()
{
}

const string CClientAppLogic::getData()
{
	return machine_data->getJSON();
}

void CClientAppLogic::run()
{
	// update machine data 
	machine_data->update();

	// update server and key configuration data 
	m_server_config.update();

	try
	{
		boost::asio::io_service io_service;
		CClientConnection c(io_service, m_server_config.getIp(), m_server_config.getPort(), getData());
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
}
