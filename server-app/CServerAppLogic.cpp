#include "CServerAppLogic.h"
#include "CServerConnection.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#define port 9999

CServerAppLogic::CServerAppLogic()
{
}


CServerAppLogic::~CServerAppLogic()
{
}

void CServerAppLogic::run()
{
	try
	{
		boost::asio::io_service io_service;

		using namespace std; // For atoi.
		CServerConnection s(io_service, port);

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

void CServerAppLogic::loadClientConfigurations()
{
}

void CServerAppLogic::writeDatabase(string data)
{

}

void CServerAppLogic::receivedPackage(session* s, string data, size_t bytes_transferred)
{
	// Unpack package

	// check key

	// send anwser
	string resp = getResponseData();
	s->send_data(resp, resp.length());

	// write db
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
	ssOut << "HTTP/1.1 200 OK" << std::endl;
	ssOut << "Content-Type: text/html" << std::endl;
	ssOut << "Content-Length: " << json.length() << std::endl;
	ssOut << std::endl;
	ssOut << json;

	return ssOut.str();
}