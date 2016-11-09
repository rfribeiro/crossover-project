#pragma once
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <CLogger.h>

using boost::asio::ip::tcp;
using namespace std;

enum ConnectionStatus
{
	CS_ERROR = -1,
	CS_SUCCESS = 1
};

class CClientConnection
{
public:
	CClientConnection(boost::asio::io_service& io_service,
		const std::string& server, const std::string& port, const std::string& data)
		: resolver_(io_service),
		socket_(io_service)
	{
		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		std::ostream request_stream(&request_);
		request_stream << "POST /title/ HTTP/1.1 \r\n";
		request_stream << "Host:" << server << "\r\n";
		request_stream << "User-Agent: C/1.0";
		request_stream << "Content-Type: application/json; charset=utf-8 \r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Content-Length: " << data.length() << "\r\n";
		request_stream << "Connection: close\r\n\r\n";  //NOTE THE Double line feed
		request_stream << data;

		// Start an asynchronous resolve to translate the server and service names
		// into a list of endpoints.
		tcp::resolver::query query(server, port);
		resolver_.async_resolve(query,
			boost::bind(&CClientConnection::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator));
	}

private:
	std::string make_string(boost::asio::streambuf& streambuf)
	{
		return{ buffers_begin(streambuf.data()),
			buffers_end(streambuf.data()) };
	}

	void handle_resolve(const boost::system::error_code& err,
		tcp::resolver::iterator endpoint_iterator)
	{
		if (!err)
		{
			// Attempt a connection to each endpoint in the list until we
			// successfully establish a connection.
			boost::asio::async_connect(socket_, endpoint_iterator,
				boost::bind(&CClientConnection::handle_connect, this,
					boost::asio::placeholders::error));
		}
		else
		{
			LOG_ERROR << err.message();
		}
	}

	void handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			LOG_INFO << "Sent : " << endl << make_string(request_) << endl << endl;

			// The connection was successful. Send the request.
			boost::asio::async_write(socket_, request_,
				boost::bind(&CClientConnection::handle_write_request, this,
					boost::asio::placeholders::error));
		}
		else
		{
			LOG_ERROR << err.message();
		}
	}

	void handle_write_request(const boost::system::error_code& err)
	{
		if (!err)
		{
			// Read the response status line. The response_ streambuf will
			// automatically grow to accommodate the entire line. The growth may be
			// limited by passing a maximum size to the streambuf constructor.
			boost::asio::async_read_until(socket_, response_, "\r\n",
				boost::bind(&CClientConnection::handle_read_status_line, this,
					boost::asio::placeholders::error));
		}
		else
		{
			LOG_ERROR << err.message();
		}
	}

	void handle_read_status_line(const boost::system::error_code& err)
	{
		if (!err)
		{
			LOG_INFO << "Received : " << endl << make_string(response_) << endl << endl;

			// Check that response is OK.
			std::istream response_stream(&response_);
			std::string http_version;

			response_stream >> http_version;
			unsigned int status_code;
			response_stream >> status_code;
			std::string status_message;
			std::getline(response_stream, status_message);
			if (!response_stream || http_version.substr(0, 5) != "HTTP/")
			{
				LOG_ERROR << "Invalid response\n";
				return;
			}
			if (status_code != 200)
			{
				LOG_ERROR << "Response returned with status code " << status_code;
				return;
			}

			// Read the response headers, which are terminated by a blank line.
			boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
				boost::bind(&CClientConnection::handle_read_headers, this,
					boost::asio::placeholders::error));

		}
		else
		{
			LOG_ERROR << err;
		}
	}

	void handle_read_headers(const boost::system::error_code& err)
	{
		if (!err)
		{
			int content_length = 0;
			// Process the response headers.
			std::istream response_stream(&response_);
			std::string header;
			while (std::getline(response_stream, header) && header != "\r")
			{
				if (strncmp(header.c_str(), "Content-Length:", strlen("Content-Length:")) == 0)
				{
					content_length = atoi(header.substr(15, header.length() - 1).c_str());
				}
				//std::cout << header << "\n";
			}
			//std::cout << "\n";

			// Write whatever content we already have to output.
			if (response_.size() > 0)
				std::cout << &response_;

			// Start reading remaining data until EOF.
			boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(0),
				boost::bind(&CClientConnection::handle_read_content, this,
					boost::asio::placeholders::error));
		}
		else
		{
			LOG_ERROR << err;
		}
	}

	void handle_read_content(const boost::system::error_code& err)
	{
		if (!err)
		{
			// Write all of the data that has been read so far.
			std::cout << &response_;

		}
		else if (err != boost::asio::error::eof)
		{
			LOG_ERROR << err;
		}
	}

	tcp::resolver resolver_;
	tcp::socket socket_;
	boost::asio::streambuf request_;
	boost::asio::streambuf response_;
};

