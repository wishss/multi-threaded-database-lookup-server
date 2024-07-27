#include <iostream>
#include "ListenerMgmt.h"

int main()
{
	ListenerMgmt server("http://localhost:8080/api");
	server.MgmtStart();

	return 0;
}