#include <boost/test/unit_test.hpp>
#include <CAlert.h>

BOOST_AUTO_TEST_CASE(CALert_validate_object)
{
	CAlert instance(CAlertType::ALERT_CPU, 100);

	BOOST_REQUIRE_EQUAL(CAlertType::ALERT_CPU, instance.getType());
	BOOST_REQUIRE_EQUAL(100, instance.getValue());
}
