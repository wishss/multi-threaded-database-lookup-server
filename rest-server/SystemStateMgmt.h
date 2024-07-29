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
#include <ctime>
#include <atomic>

#include "DBMgmt.h"

class SystemStateMgmt
{
public:
	SystemStateMgmt();
	~SystemStateMgmt();

	void startMgmt();
	void stopMgmt();

private:
	double getSystemCpuUsage();
	long getSystemMemoryUsage();
	long getSystemDiskUsage();

	long getTotalCpuTime();
	long getIdleCpuTime();
	double calculateCpuUsagePercent(long total_cpu_time, long idle_cpu_time);
	bool executeCommand(const std::string& command, std::string& output);

	std::string getCurrentDate();
	std::string getCurrentTime();

private:
	void updateSystemState();

private:
	std::atomic<bool> stop_requested_;
	long ticks_per_second_;
	DBMgmt db_mgmt_;
};