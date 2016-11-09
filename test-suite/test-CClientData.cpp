#include <boost/test/unit_test.hpp>
#include <CClientData.h>

BOOST_AUTO_TEST_CASE(CClientData_validate_object)
{
	CClientData instance;

	BOOST_CHECK(instance.getId() != 0);
	BOOST_CHECK(instance.getKey() == "");
	BOOST_CHECK(instance.getEmail() == "");
}

BOOST_AUTO_TEST_CASE(CClientData_alert_true)
{
	CClientData instance;
	CAlert alert(CAlertType::ALERT_MEMORY, 50);
	instance.addAlert(CAlertType::ALERT_MEMORY, alert);

	BOOST_CHECK(instance.checkAlert(CAlertType::ALERT_MEMORY, 50) == false);
	BOOST_CHECK(instance.checkAlert(CAlertType::ALERT_MEMORY, 100) == true);
	BOOST_CHECK(instance.checkAlert(CAlertType::ALERT_MEMORY, 40) == false);
}
