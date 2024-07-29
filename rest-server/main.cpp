#include <iostream>
#include <thread>
#include "SystemStateMgmt.h"
#include "ListenerMgmt.h"

int main()
{
    // �ý��� ���� ���� ������
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

    // Listener ���� ������
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