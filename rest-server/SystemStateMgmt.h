#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <iomanip> 
#include <thread>
#include <chrono> 

class SystemStateMgmt
{
public:
	SystemStateMgmt();
	~SystemStateMgmt();

	void startMgmt();

private:
	//시스템 상태 관련 함수
	double getSystemCpuUsage();
	long getSystemMemoryUsage();
	long getSystemDiskUsage();
	long getTotalCpuTime();
	long getIdleCpuTime();
	double calculateCpuUsagePercent(long total_cpu_time, long idle_cpu_time);
	bool executeCommand(const std::string& command, std::string& output);

private:
	/* db 접근 관련 함수 필요 */
	void updateSystemState();

private:
	long ticks_per_second_;
};