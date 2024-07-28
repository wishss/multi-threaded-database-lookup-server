#include <iostream>
#include <thread>
#include "SystemStateMgmt.h"
#include "ListenerMgmt.h"
#include "DBMgmt.h"

int main()
{
    //db test
    DBMgmt dbmgmt("test.db");
    if (!dbmgmt.open()) { return 0; }

    dbmgmt.insertData("2024-07-25", "00:00:00", 2.0, 10000, 20000);
    dbmgmt.insertData("2024-07-26", "00:00:00", 2.0, 10000, 20000);
    dbmgmt.insertData("2024-07-27", "00:00:00", 2.0, 10000, 20000);

    dbmgmt.getDataByDateRange("2024-07-25", "2024-07-27");
    

    // �ý��� ���� ���� ������ ���� ������
    std::thread systemThread([]() {
        SystemStateMgmt system_state;
        system_state.startMgmt();
        });

    // Listener ���� ������ ���� ������
    std::thread listenerThread([]() {
        ListenerMgmt listener("http://localhost:8080/api");
        listener.startMgmt();
        });

    systemThread.join();
    listenerThread.join();

	return 0;
}