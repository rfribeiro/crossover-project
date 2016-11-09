#define _SCL_SECURE_NO_WARNINGS
#include "CEmail.h"
#include <boost/lexical_cast.hpp>
#include <CLogger.h>

CEmail::CEmail(std::string pServer, unsigned int pPort, std::string pUser, std::string pPassword) :
	mServer(pServer), mPort(pPort), mUserName(pUser), mPassword(pPassword), mSocket(mIOService), mResolver(mIOService)
{
	try
	{
		tcp::resolver::query qry(mServer, boost::lexical_cast<std::string>(mPort));
		mResolver.async_resolve(qry, boost::bind(&CEmail::handleResolve, this, boost::asio::placeholders::error,
			boost::asio::placeholders::iterator));
	}
	catch (std::exception& ex)
	{
		LOG_ERROR << "Init SMTP connection : " << ex.what();
	}
}

CEmail::CEmail(std::string pServer, std::string pPort, std::string pUser, std::string pPassword)
	: mServer(pServer), mUserName(pUser), mPassword(pPassword), mSocket(mIOService), mResolver(mIOService)
{
	try
	{
		mPort = boost::lexical_cast<unsigned int>(pPort);
	}
	catch (boost::bad_lexical_cast const& l)
	{
		mPort = 25;
		LOG_ERROR << l.what();
	}
	catch (std::exception& ex) 
	{
		LOG_ERROR << ex.what();
	}
}

bool CEmail::send(std::string pFrom, std::string pTo, std::string pSubject, std::string pMessage)
{
	mFrom = pFrom;
	mTo = pTo;
	mSubject = pSubject;
	mMessage = pMessage;
	mIOService.run();
	return mHasError;
}

std::string CEmail::encodeBase64(std::string pData)
{
	std::stringstream os;
	size_t sz = pData.size();
	std::copy(base64_text(pData.c_str()), base64_text(pData.c_str() + sz), ostream_iterator<char>(os));
	//std::copy(base64_text(pData.c_str()), base64_text(pData.c_str() + sz), stdext::checked_array_iterator<ostream_iterator>(os, pData.c_str() + sz));
	return os.str();
}
void CEmail::handleResolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{
		tcp::endpoint endpoint = *endpoint_iterator;
		mSocket.async_connect(endpoint,
			boost::bind(&CEmail::handleConnect, this, boost::asio::placeholders::error, ++endpoint_iterator));
	}
	else
	{
		mHasError = true;
		mErrorMsg = err.message();
		LOG_ERROR << mErrorMsg;
	}
}
void CEmail::writeLine(std::string pData)
{
	std::ostream req_strm(&mRequest);
	req_strm << pData << "\r\n";
	boost::asio::write(mSocket, mRequest);
	req_strm.clear();
}
void CEmail::handleConnect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{
		// The connection was successful. Send the request.
		std::ostream req_strm(&mRequest);
		writeLine("EHLO " + mServer);
		writeLine("AUTH LOGIN");
		writeLine(encodeBase64(mUserName));
		writeLine(encodeBase64(mPassword));
		writeLine("MAIL FROM:<" + mFrom + ">");
		writeLine("RCPT TO:<" + mTo + ">");
		writeLine("DATA");
		writeLine("SUBJECT:" + mSubject);
		writeLine("From:" + mFrom);
		writeLine("To:" + mTo);
		writeLine("");
		writeLine(mMessage);
		writeLine(".\r\n");
	}
	else
	{
		mHasError = true;
		mErrorMsg = err.message();
		LOG_ERROR << mErrorMsg;
	}
}
