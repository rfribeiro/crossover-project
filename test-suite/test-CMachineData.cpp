#include <boost/test/unit_test.hpp>
#include <CMachineData.h>


BOOST_AUTO_TEST_CASE(CMachineData_validate_object)
{
	CMachineData instance = CMachineData(0, "key", 100, 55, 10, "1000");

	BOOST_REQUIRE_EQUAL(0, instance.getId());
	BOOST_CHECK(std::string("key")==instance.getKey());
	BOOST_CHECK(100== instance.getMemory());
	BOOST_CHECK(55 == instance.getCpuUsage());
	BOOST_CHECK(10 == instance.getProcess());
	BOOST_CHECK(std::string("1000")==instance.getTimestamp());
}

BOOST_AUTO_TEST_CASE(CMachineData_validate_update)
{
	CMachineData instance = CMachineData(0, "key", 100, 55, 10, "1000");
	instance.update();

	BOOST_REQUIRE(instance.getId() == 0);
	BOOST_CHECK("key" == instance.getKey());
	BOOST_CHECK(instance.getKey() != "");
	BOOST_CHECK(instance.getMemory() > 0);
	BOOST_CHECK(instance.getCpuUsage() > 0);
	BOOST_CHECK(instance.getProcess() > 0);
	BOOST_CHECK("1000" != instance.getTimestamp());
}