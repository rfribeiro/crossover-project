#include <boost/test/unit_test.hpp>
#include <CServerConfiguration.h>

BOOST_AUTO_TEST_CASE(CServerConfiguration_validate_object)
{
	CServerConfiguration instance;

	BOOST_CHECK(instance.getServerPort() == 9999);
	BOOST_CHECK(instance.getSmtpAddress() == "smtp.gmail.com");
	BOOST_CHECK(instance.getSmtpPort() == "587");
	BOOST_CHECK(instance.getSmtpUserEmail() == "rfribeiro.crossover@gmail.com");
	BOOST_CHECK(instance.getSmtpUserPassword() == "!1qaz@2wsx");

}

BOOST_AUTO_TEST_CASE(CServerConfiguration_read)
{
	CServerConfiguration instance;

	BOOST_REQUIRE_NO_THROW(instance.update());
}
