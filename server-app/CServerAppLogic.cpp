#include "CServerAppLogic.h"
#include "CServerConnection.h"
#include "CEmail.h"
#include "../common/CDBMachineData.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <sstream>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/scoped_thread.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

#define port 9999
#define JSON_INDEX 160

const string string_client_id = "id";
const string string_key = "key";
const string string_memory = "memory";
const string string_cpu = "cpu";
const string string_processes = "processes";
const string string_timestamp = "timestamp";

CServerAppLogic::CServerAppLogic()
{
	loadClientConfigurations();
}


CServerAppLogic::~CServerAppLogic()
{
}

void CServerAppLogic::processData()
{
	while (true)
	{
		BOOST_LOG_TRIVIAL(trace) << "processData thread started!";

		Singleton<CServerAppLogic>::Instance()->readAndSaveData();

		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}
}

void CServerAppLogic::processRequests()
{
	while (true)
	{
		BOOST_LOG_TRIVIAL(trace) << "processRequests thread started!";

		//start server receive data
		boost::asio::io_service io_service;
		CServerConnection s(io_service, port);
		io_service.run();

		boost::this_thread::sleep(boost::posix_time::seconds(60));
	}
}

void CServerAppLogic::run()
{
	try
	{
		// start read and save thread
		boost::thread td{ CServerAppLogic::processData };

		// start read and save thread
		boost::thread tr{ CServerAppLogic::processRequests };

		td.join();
		tr.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

void CServerAppLogic::loadClientConfigurations()
{
	BOOST_LOG_TRIVIAL(trace) << "loadClientConfigurations";
}

void CServerAppLogic::writeDatabase(string data)
{

}

CMachineData* CServerAppLogic::mapHttpToObject(string data)
{
	CMachineData* new_data = NULL;
	try {
		ptree pt2;
		std::istringstream is(data);
		read_json(is, pt2);
		
		int id = pt2.get<int>(string_client_id, 0);
		std::string key = pt2.get<std::string>(string_key);
		std::string timestamp = pt2.get<std::string>(string_timestamp);
		double memory = pt2.get<double>(string_memory);
		double cpu = pt2.get<double>(string_cpu);
		double process = pt2.get<double>(string_processes);

		new_data = new CMachineData(id,key, memory, cpu, process, timestamp);
	}
	catch (std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << "mapHttpToObject Exception: " << e.what() << "\n";
	}
	
	return new_data;
}

void CServerAppLogic::receivedPackage(string data)
{
	try
	{
		// save data
		if (data.length() > 160)
		{
			string json_data = data.substr(JSON_INDEX, data.length() - 1);
			m_queue.push(json_data);
		}
	}
	catch (std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what() << "\n";
	}
}

void CServerAppLogic::readAndSaveData()
{
	string data;
	CMachineData* machine_data = NULL;

	if (!m_queue.empty())
	{
		// Unpack package
		m_queue.wait_and_pop(data);
		// check key
		machine_data = mapHttpToObject(data);
		if (machine_data != NULL)
		{
			if (validateKey(machine_data))
			{
				// write db
				writeDatabase(machine_data);

				// check alerts
				checkAlerts(machine_data);
			}
			else
			{
				// Invalid key
			}
			delete machine_data;
		}
		machine_data = NULL;
	}
	else
	{
		BOOST_LOG_TRIVIAL(trace) << "readAndSaveData: Queue empty";
	}
}

void CServerAppLogic::writeDatabase(CMachineData* data)
{
	CDBMachineData database;

	if (database.open() == DBConnectionStatus::CONNECTED)
	{
		if (database.write(data) == DBDataStatus::WRITE_OK)
		{
			if (database.close() == DBConnectionStatus::DISCONNECTED)
			{
				BOOST_LOG_TRIVIAL(trace) << "writeDatabase: Success";
				return;
			}
		}
	}
	BOOST_LOG_TRIVIAL(error) << "writeDatabase: Error";
}

void CServerAppLogic::checkAlerts(CMachineData* data)
{
	CClientData client = m_clients[data->getId()];
	std::stringstream resp;
	bool bNeedEmail = false;

	if (client.checkAlert(CAlertType::ALERT_MEMORY, data->getMemory()))
	{
		resp << "Memory Alert: " << data->getMemory() << " : " << (client.getAlert(CAlertType::ALERT_MEMORY)).getValue() << endl;
		bNeedEmail = true;
	}

	if (client.checkAlert(CAlertType::ALERT_CPU, data->getCpuUsage()))
	{
		resp << "CPU Alert: " << data->getCpuUsage() << " : " << (client.getAlert(CAlertType::ALERT_CPU)).getValue() << endl;
		bNeedEmail = true;
	}

	if (client.checkAlert(CAlertType::ALERT_PROCESSES, data->getProcess()))
	{
		resp << "Processes Alert: " << data->getProcess() << " : " << (client.getAlert(CAlertType::ALERT_PROCESSES)).getValue() << endl;
		bNeedEmail = true;
	}

	if (bNeedEmail)
	{
		sendEmail(client, resp.str());
	}
}

bool CServerAppLogic::validateKey(CMachineData * data)
{
	return false;
}

bool CServerAppLogic::sendEmail(CClientData client, string data)
{
	CEmail mailc("yoursmtpserver.com", 25, "user@yourdomain.com", "password");
	
	if (mailc.send("from@yourdomain.com", client.getEmail(), "Alert Message", data))
	{
		BOOST_LOG_TRIVIAL(error) << "sendEmail: Error";
		return false;
	}

	BOOST_LOG_TRIVIAL(trace) << "sendEmail: Success";
	return true;
}

void CServerAppLogic::sendPackage()
{
	// get response package

	// send package

}

string CServerAppLogic::getResponseData()
{
	std::stringstream ssOut;
	std::string json = "{ \"success\" : \"true\" }";
	ssOut << "HTTP/1.1 200 OK" << "\r\n";
	ssOut << "Content-Type: text/html" << "\r\n";
	ssOut << "Content-Length: " << json.length();
	ssOut << "\r\n\r\n";
	ssOut << json;

	return ssOut.str();
}


