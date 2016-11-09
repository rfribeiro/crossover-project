#include <boost/test/unit_test.hpp>
#include <CClientConfiguration.h>

BOOST_AUTO_TEST_CASE(CClientConfiguration_validate_object)
{
	CClientConfiguration instance;

	BOOST_CHECK(instance.getIp() == "localhost");
	BOOST_CHECK(instance.getKey() == "xxxx");
	BOOST_CHECK(instance.getTimer() == 5);
}

BOOST_AUTO_TEST_CASE(CClientConfiguration_read)
{
	CClientConfiguration instance;

	BOOST_REQUIRE_NO_THROW(instance.update());
}
