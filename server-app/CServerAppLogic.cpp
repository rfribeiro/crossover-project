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
	// log

	// Unpack package

	// check key

	// write db

	// check alerts
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