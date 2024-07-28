#include <iostream>
#include <thread>
#include "SystemStateMgmt.h"
#include "ListenerMgmt.h"

int main()
{
    // 시스템 상태 관리 시작을 위한 스레드
    std::thread systemThread([]() {
        SystemStateMgmt system_state;
        system_state.startMgmt();
        });

    // Listener 관리 시작을 위한 스레드
    std::thread listenerThread([]() {
        ListenerMgmt listener("http://localhost:8080/api");
        listener.startMgmt();
        });

    systemThread.join();
    listenerThread.join();

	return 0;
}