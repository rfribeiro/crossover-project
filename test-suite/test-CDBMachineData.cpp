#include <boost/test/unit_test.hpp>
#include <CDBMachineData.h>
#include <CMachineData.h>
	
BOOST_AUTO_TEST_CASE(test_connection)
{
	CDBMachineData instance;
	BOOST_REQUIRE(DBConnectionStatus::CONNECTED == instance.open());
	BOOST_REQUIRE(DBConnectionStatus::DISCONNECTED == instance.close());
}

BOOST_AUTO_TEST_CASE(test_write_data)
{
	CDBMachineData instance;
	CMachineData* data = new CMachineData(1, "test", 100, 100, 100);
	BOOST_REQUIRE(DBConnectionStatus::CONNECTED == instance.open());
	BOOST_REQUIRE(DBDataStatus::WRITE_OK == instance.write(data));
	BOOST_REQUIRE(DBConnectionStatus::DISCONNECTED ==  instance.close());
	delete data;
}