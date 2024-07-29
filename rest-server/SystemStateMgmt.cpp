#include "SystemStateMgmt.h"

SystemStateMgmt::SystemStateMgmt()
    :stop_requested_(false)
{
    // 1초당 클럭 틱 수
    ticks_per_second_ = sysconf(_SC_CLK_TCK);
    if (ticks_per_second_ == -1) {
        throw std::runtime_error("Failed to determine clock ticks per second.");
    }
}

SystemStateMgmt::~SystemStateMgmt()
{
    stopMgmt();
    db_mgmt_.close();
}

void SystemStateMgmt::startMgmt() {
    if (!db_mgmt_.open("state.db")) { throw std::runtime_error("Failed to open database."); }

    std::cout << "Started system state monitoring. Interval: 1 minute." << std::endl;

    while (!stop_requested_.load()) {
        // 1분마다 시스템 상태 업데이트
        updateSystemState();
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
}

void SystemStateMgmt::stopMgmt() {
    stop_requested_.store(true);
}

double SystemStateMgmt::getSystemCpuUsage() {
    long total_cpu_time = getTotalCpuTime();
    long idle_cpu_time = getIdleCpuTime();

    return calculateCpuUsagePercent(total_cpu_time, idle_cpu_time);
}

long SystemStateMgmt::getSystemMemoryUsage() {
    // 메모리 사용량 추출
    std::ifstream mem_info("/proc/meminfo");
    if (!mem_info.is_open()) {
        std::cerr << "Failed to open /proc/meminfo" << std::endl;
        return -1;
    }

    long total_mem = 0, free_mem = 0;
    std::string line;
    while (std::getline(mem_info, line)) {
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
    mem_info.close();

    return total_mem - free_mem;
}

long SystemStateMgmt::getSystemDiskUsage() {
    // 디스크 사용량 추출
    std::string command_output;
    if (!executeCommand("df -k --total | tail -n 1", command_output)) {
        std::cerr << "Failed to execute 'df' command." << std::endl;
        return -1;
    }

    std::istringstream iss(command_output);
    std::string file_system;
    long total, used, available;
    iss >> file_system >> total >> used >> available;

    return used;
}

long SystemStateMgmt::getTotalCpuTime() {
    // CPU의 총 사용 시간 추출
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
    iss >> cpu_label;
    if (cpu_label != "cpu") {
        std::cerr << "Unexpected line format: " << line << std::endl;
        return -1;
    }

    long user, nice, system, idle;
    iss >> user >> nice >> system >> idle;

    return user + nice + system + idle;
}

long SystemStateMgmt::getIdleCpuTime() {
    // CPU의 idle 시간 추출
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
    iss >> cpu_label;
    if (cpu_label != "cpu") {
        std::cerr << "Unexpected line format: " << line << std::endl;
        return -1;
    }

    long user, nice, system, idle;
    iss >> user >> nice >> system >> idle;

    return idle;
}

double SystemStateMgmt::calculateCpuUsagePercent(long total_cpu_time, long idle_cpu_time) {
    static long prev_total_cpu_time = 0;
    static long prev_idle_cpu_time = 0;

    // 시간 차이 계산
    long delta_total = total_cpu_time - prev_total_cpu_time;
    long delta_idle = idle_cpu_time - prev_idle_cpu_time;

    // 현재 CPU 시간 값을 저장
    prev_total_cpu_time = total_cpu_time;
    prev_idle_cpu_time = idle_cpu_time;

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

std::string SystemStateMgmt::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d");  // Format: YYYY-MM-DD
    return oss.str();
}
std::string SystemStateMgmt::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(now_tm, "%H:%M:%S");  // Format: HH:MM:SS
    return oss.str();
}

void SystemStateMgmt::updateSystemState() {
    // 현재 날짜와 시간 가져오기
    std::string date = getCurrentDate();
    std::string time = getCurrentTime();

    // 시스템 상태 get
    double cpu_usage_percent = getSystemCpuUsage();
    long memory_usage_kb = getSystemMemoryUsage();
    long disk_usage_kb = getSystemDiskUsage();

    // db 삽입
    db_mgmt_.insertData(date, time, cpu_usage_percent, memory_usage_kb, disk_usage_kb);
}