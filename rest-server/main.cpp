#include <iostream>
#include <thread>
#include "SystemStateMgmt.h"
#include "ListenerMgmt.h"
#include "DBMgmt.h"  // 필요없음

int main()
{
	DBMgmt db_mgmt_;
	db_mgmt_.open("state.db");
	db_mgmt_.insertData("2024-07-26", "00:00:00", 0.2, 1000, 10000);
	db_mgmt_.insertData("2024-07-26", "00:01:00", 0.2, 1000, 10000);
	db_mgmt_.insertData("2024-07-26", "00:02:00", 0.2, 1000, 10000);

	db_mgmt_.insertData("2024-07-27", "00:00:00", 0.2, 1000, 10000);
	db_mgmt_.insertData("2024-07-27", "00:01:00", 0.2, 1000, 10000);
	db_mgmt_.insertData("2024-07-27", "00:02:00", 0.2, 1000, 10000);

	db_mgmt_.insertData("2024-07-28", "00:00:00", 0.2, 1000, 10000);
	db_mgmt_.insertData("2024-07-28", "00:01:00", 0.2, 1000, 10000);
	db_mgmt_.insertData("2024-07-28", "00:02:00", 0.2, 1000, 10000);

	db_mgmt_.close();

    // 시스템 상태 관리 스레드
	std::thread systemThread([]() {
		try {
			SystemStateMgmt system_state;
			system_state.startMgmt();
		}
		catch (const std::exception& e) {
			std::cerr << "Exception in system thread: " << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Unknown exception in system thread." << std::endl;
		}
		});

    // Listener 관리 스레드
	std::thread listenerThread([]() {
		try {
			ListenerMgmt listener("http://localhost:8080/api");
			listener.startMgmt();
		}
		catch (const std::exception& e) {
			std::cerr << "Exception in listener thread: " << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Unknown exception in listener thread." << std::endl;
		}
		});

    systemThread.join();
    listenerThread.join();

	return 0;
}