#include <iostream>

using namespace std;

#include "CClientAppLogic.h"
#include <CLogger.h>

int main(void) 
{
	LOG_INFO << "Client Program started";

	CClientAppLogic* logic = Singleton<CClientAppLogic>::Instance();
	logic->init();

	return 0;
}