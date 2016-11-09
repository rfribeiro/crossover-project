#include <boost/test/unit_test.hpp>
#include <CEmail.h>

BOOST_AUTO_TEST_CASE(CEmail_read)
{
	BOOST_REQUIRE_NO_THROW(CEmail instance("test", 1, "user_test", "pass_test"));
}
