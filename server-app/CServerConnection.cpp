#include "CServerConnection.h"
#include "../common/Singleton.h"
#include "CServerAppLogic.h"

tcp::socket& session::socket()
{
	return socket_;

	std::ostream request_stream(&request_);
	request_stream << Singleton<CServerAppLogic>::Instance()->getResponseData();
}

void session::start()
{
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
		boost::bind(&session::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void session::send_data(string data_to_transfer)
{

}

void session::handle_read(const boost::system::error_code& error,
	size_t bytes_transferred)
{
	if (!error)
	{
		cout << "Received: " << endl << data_ << endl << endl;

		CServerAppLogic* logic = Singleton<CServerAppLogic>::Instance();
		logic->receivedPackage(data_);
		string data_resp = logic->getResponseData();

		if (data_resp.length() < sizeof(data_))
		{
			memset(data_, 0, sizeof(data_));
			memcpy(data_, data_resp.c_str(), data_resp.length());
		}

		boost::asio::async_write(socket_,
			boost::asio::buffer(data_, data_resp.length()),
			boost::bind(&session::handle_write, this,
				boost::asio::placeholders::error));
	}
	else
	{
		delete this;
	}
}

void session::handle_write(const boost::system::error_code& error)
{
	if (!error)
	{
		cout << "Sent: " << endl << data_ << endl << endl;

		socket_.async_read_some(boost::asio::buffer(data_, max_length),
			boost::bind(&session::handle_read, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		delete this;
	}
}
