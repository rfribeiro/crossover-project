#pragma once
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
using boost::asio::ip::tcp;
using namespace boost::archive::iterators;
typedef base64_from_binary<transform_width<const char *, 6, 8> > base64_text;

class CEmail
{
public:
	CEmail(std::string pServer, unsigned int pPort, std::string pUser, std::string pPassword);
	bool send(std::string pFrom, std::string pTo, std::string pSubject, std::string pMessage);
private:
	std::string encodeBase64(std::string pData);
	void handleResolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void writeLine(std::string pData);
	void handleConnect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);

	std::string mServer;
	std::string mUserName;
	std::string mPassword;
	std::string mFrom;
	std::string mTo;
	std::string mSubject;
	std::string mMessage;
	unsigned int mPort;
	boost::asio::io_service mIOService;
	tcp::resolver mResolver;
	tcp::socket mSocket;
	boost::asio::streambuf mRequest;
	boost::asio::streambuf mResponse;
	bool mHasError;
	std::string mErrorMsg;

};