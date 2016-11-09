#include <iostream>
#include "CServerAppLogic.h"
#include <CLogger.h>

using namespace std;


int main()
{
	LOG_INFO << "Server Program started";

	CServerAppLogic* logic = Singleton<CServerAppLogic>::Instance();
	logic->run();

	return 0;
}