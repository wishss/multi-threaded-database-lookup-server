#include "SystemStateMgmt.h"

SystemStateMgmt::SystemStateMgmt()
{
    // Determine clock ticks per second for CPU time calculations
    ticks_per_second_ = sysconf(_SC_CLK_TCK);
    if (ticks_per_second_ == -1) {
        std::cerr << "Failed to determine clock ticks per second." << std::endl;
        // Handle error if needed
    }
}

SystemStateMgmt::~SystemStateMgmt()
{

}

void SystemStateMgmt::startMgmt() {
    /*db 연결 안되면 실행x*/
    while (true) {
        // Update and print system state every 1 second
        updateSystemState();
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
}

double SystemStateMgmt::getSystemCpuUsage() {
    long total_cpu_time = getTotalCpuTime();
    long idle_cpu_time = getIdleCpuTime();

    return calculateCpuUsagePercent(total_cpu_time, idle_cpu_time);
}

long SystemStateMgmt::getSystemMemoryUsage() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        std::cerr << "Failed to open /proc/meminfo" << std::endl;
        return -1;
    }

    long total_mem = 0, free_mem = 0;
    std::string line;
    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        if (iss >> key) {
            if (key == "MemTotal:") {
                iss >> total_mem;
            }
            else if (key == "MemFree:" || key == "Buffers:" || key == "Cached:") {
                long value;
                iss >> value;
                free_mem += value;
            }
        }
    }
    meminfo.close();

    return total_mem - free_mem;
}

long SystemStateMgmt::getSystemDiskUsage() {
    // Use df command to get disk usage
    std::string command_output;
    if (!executeCommand("df -k --total | tail -n 1", command_output)) {
        std::cerr << "Failed to execute 'df' command." << std::endl;
        return -1;
    }

    std::istringstream iss(command_output);
    std::string filesystem;
    long total, used, available;
    iss >> filesystem >> total >> used >> available;

    return used;
}

long SystemStateMgmt::getTotalCpuTime() {
    std::ifstream proc_stat("/proc/stat");
    if (!proc_stat.is_open()) {
        std::cerr << "Failed to open /proc/stat" << std::endl;
        return -1;
    }

    std::string line;
    std::getline(proc_stat, line);
    proc_stat.close();

    std::istringstream iss(line);
    std::string cpu_label;
    iss >> cpu_label;  // Read "cpu" label
    long user, nice, system, idle;
    iss >> user >> nice >> system >> idle;

    return user + nice + system + idle;
}

long SystemStateMgmt::getIdleCpuTime() {
    std::ifstream proc_stat("/proc/stat");
    if (!proc_stat.is_open()) {
        std::cerr << "Failed to open /proc/stat" << std::endl;
        return -1;
    }

    std::string line;
    std::getline(proc_stat, line);
    proc_stat.close();

    std::istringstream iss(line);
    std::string cpu_label;
    iss >> cpu_label;  // Read "cpu" label
    long user, nice, system, idle;
    iss >> user >> nice >> system >> idle;

    return idle;
}

double SystemStateMgmt::calculateCpuUsagePercent(long total_cpu_time, long idle_cpu_time) {
    static long prev_total_cpu_time = 0;
    static long prev_idle_cpu_time = 0;

    // Calculate differences since last call
    long delta_total = total_cpu_time - prev_total_cpu_time;
    long delta_idle = idle_cpu_time - prev_idle_cpu_time;

    // Update previous values for next calculation
    prev_total_cpu_time = total_cpu_time;
    prev_idle_cpu_time = idle_cpu_time;

    double cpu_usage_percent = 100.0 * (1.0 - (static_cast<double>(delta_idle) / delta_total));

    return double(100.0 * (1.0 - (static_cast<double>(delta_idle) / delta_total)));
}

bool SystemStateMgmt::executeCommand(const std::string& command, std::string& output) {
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return false;
    }
    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
            output += buffer;
        }
    }
    pclose(pipe);
    return true;
}

void SystemStateMgmt::updateSystemState() {
    // Update system state here
    double cpu_usage_percent = getSystemCpuUsage();
    long memory_usage_kb = getSystemMemoryUsage();
    long disk_usage_kb = getSystemDiskUsage();

    // Print system state
    std::cout << "System CPU Usage: " << std::fixed << std::setprecision(2) << cpu_usage_percent << "%" << std::endl;
    std::cout << "System Memory Usage: " << memory_usage_kb << " KB" << std::endl;
    std::cout << "System Disk Usage: " << disk_usage_kb << " KB" << std::endl;
}