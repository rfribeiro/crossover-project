#include "CServerAppLogic.h"
#include "CServerConnection.h"
#include "CServerConfiguration.h"

#include "CEmail.h"
#include "../common/CDBMachineData.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <sstream>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/scoped_thread.hpp>

#include <CLogger.h>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
namespace pt = boost::property_tree;

#define JSON_INDEX 159
#define CLIENTS_CONF_FILENAME "clients-data.xml"

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
	LOG_INFO << "processData thread started";
	while (true)
	{
		Singleton<CServerAppLogic>::Instance()->readAndSaveData();

		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}
}

void CServerAppLogic::processRequests()
{
	LOG_INFO << "processRequests thread started";

	while (true)
	{
		//start server receive data
		boost::asio::io_service io_service;
		CServerConnection s(io_service, Singleton<CServerConfiguration>::Instance()->getServerPort());
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
		LOG_ERROR << e.what();
	}
}

void CServerAppLogic::loadClientConfigurations()
{
	try
	{
		std::string filename(CLIENTS_CONF_FILENAME);

		// Create empty property tree object
		pt::ptree tree;

		// Parse the XML into the property tree.
		pt::read_xml(filename, tree, boost::property_tree::xml_parser::trim_whitespace);

		if (!tree.empty())
		{
			int client_ids = 0;
			// Use `get_child` to find the node containing the books and
			// iterate over its children.
			// `BOOST_FOREACH()` would also work.
			for (const auto &client : tree.get_child("clients"))
			{
				std::string name;
				ptree sub_pt;
				std::tie(name, sub_pt) = client;

				if (name == "client")
				{
					//int id = 0;
					//id = sub_pt.get<int>("<xmlattr>.id");
					CClientData cl;

					//cl.setId(id);
					cl.setKey(sub_pt.get<std::string>("<xmlattr>.key"));
					cl.setEmail(sub_pt.get<std::string>("<xmlattr>.mail"));

					for (const auto &alert : client.second)
					{
						std::string namealert;
						ptree sub_ptalert;
						std::tie(namealert, sub_ptalert) = alert;

						if (namealert == "<xmlattr>") continue;

						CAlert al = CAlert(sub_ptalert.get<std::string>("<xmlattr>.type"), sub_ptalert.get<std::string>("<xmlattr>.limit"));
						cl.addAlert(al.getType(), al);
					}
					
					++client_ids;
					m_clients[client_ids] = cl;
					m_map_clients[cl.getKey()] = client_ids;
				}
			}
		}
	}
	catch (std::exception& e)
	{
		LOG_ERROR << e.what();
	}
}

CMachineData* CServerAppLogic::mapHttpToObject(string data)
{
	CMachineData* new_data = NULL;
	try {
		ptree pt2;
		std::istringstream is(data);
		read_json(is, pt2);
		
		//int id = pt2.get<int>(string_client_id, 0); // not used anymore
		std::string key = pt2.get<std::string>(string_key);
		std::string timestamp = pt2.get<std::string>(string_timestamp);
		double memory = pt2.get<double>(string_memory);
		double cpu = pt2.get<double>(string_cpu);
		double process = pt2.get<double>(string_processes);

		new_data = new CMachineData(0,key, memory, cpu, process, timestamp);
	}
	catch (std::exception& e)
	{
		LOG_ERROR << e.what();
	}
	
	return new_data;
}

void CServerAppLogic::receivedPackage(string data)
{
	try
	{
		// save data
		if (data.length() > JSON_INDEX)
		{
			string json_data = data.substr(JSON_INDEX, data.length() - 1);
			m_queue.push(json_data);
		}
	}
	catch (std::exception& e)
	{
		LOG_ERROR << e.what();
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
				LOG_INFO << "writeDatabase success";
				return;
			}
		}
	}
	LOG_ERROR << "writeDatabase error";
}

void CServerAppLogic::checkAlerts(CMachineData* data)
{
	try
	{
		CClientData client = m_clients[m_map_clients[data->getKey()]];
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
	catch (std::exception& e)
	{
		LOG_ERROR << e.what();
	}
}

bool CServerAppLogic::validateKey(CMachineData * data)
{
	try
	{
		CClientData client = m_clients[m_map_clients[data->getKey()]];

		if (client.getKey().compare(data->getKey()) == 0)
			return true;
	}
	catch (std::exception& e)
	{
		LOG_ERROR << e.what();
	}
	return false;
}

bool CServerAppLogic::sendEmail(CClientData client, string data)
{
	CServerConfiguration* server_conf = Singleton<CServerConfiguration>::Instance();

	CEmail mailc(server_conf->getSmtpAddress(), server_conf->getSmtpPort(), 
		server_conf->getSmtpUserEmail(), server_conf->getSmtpUserPassword());
	
	if (mailc.send(server_conf->getSmtpUserEmail(), client.getEmail(), "Alert Message", data))
	{
		LOG_ERROR << "sendEmail: Error";
		return false;
	}

	LOG_ERROR << "sendEmail: Success";
	return true;
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


