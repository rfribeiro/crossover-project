#pragma once
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

class session
{
public:
	session(boost::asio::io_service& io_service)
		: socket_(io_service) {};

	tcp::socket& socket();

	void start();

	void send_data(string data_to_transfer, size_t bytes_to_transfer);
private:
	void handle_read(const boost::system::error_code& error,
		size_t bytes_transferred);

	void handle_write(const boost::system::error_code& error);

	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
};

class CServerConnection
{
public:
	CServerConnection(boost::asio::io_service& io_service, short port)
		: io_service_(io_service),
		acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		session* new_session = new session(io_service_);
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&CServerConnection::handle_accept, this, new_session,
				boost::asio::placeholders::error));
	}

	void handle_accept(session* new_session,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			new_session->start();
		}
		else
		{
			delete new_session;
		}

		start_accept();
	}

	boost::asio::io_service& io_service_;
	tcp::acceptor acceptor_;
};