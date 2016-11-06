#include "CServerConnection.h"
#include "../common/Singleton.h"
#include "CServerAppLogic.h"

tcp::socket& session::socket()
{
	return socket_;
}

void session::start()
{
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
		boost::bind(&session::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void session::send_data(string data_to_transfer, size_t bytes_to_transfer)
{
	boost::asio::async_write(socket_,
		boost::asio::buffer(data_to_transfer.c_str(), bytes_to_transfer),
		boost::bind(&session::handle_write, this,
			boost::asio::placeholders::error));
}

void session::handle_read(const boost::system::error_code& error,
	size_t bytes_transferred)
{
	if (!error)
	{
		Singleton<CServerAppLogic>::Instance()->receivedPackage(this, data_, bytes_transferred);
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
