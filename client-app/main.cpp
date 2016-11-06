#include <iostream>
using namespace std;

#include "CClientAppLogic.h"
#include <boost/asio.hpp>

boost::asio::io_service io_service;
boost::posix_time::seconds interval(5); // 1 second
boost::asio::deadline_timer timer(io_service, interval);

void tick(const boost::system::error_code&) 
{
	CClientAppLogic* logic = Singleton<CClientAppLogic>::Instance();
	logic->run();

	timer.expires_at(timer.expires_at() + interval);
	timer.async_wait(tick);
}

int main(void) 
{
	timer.async_wait(tick);
	io_service.run();
	return 0;
}