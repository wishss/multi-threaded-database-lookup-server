#include <iostream>
#include <thread>
#include "SystemStateMgmt.h"
#include "ListenerMgmt.h"

int main()
{
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