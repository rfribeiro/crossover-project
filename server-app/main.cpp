#include <iostream>
#include "CServerAppLogic.h"

using namespace std;

int main()
{
	CServerAppLogic* logic = Singleton<CServerAppLogic>::Instance();
	logic->run();

	return 0;
}