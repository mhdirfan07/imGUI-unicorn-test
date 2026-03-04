#include "system_monitor.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

// ==========================================
// 1. SYSTEM MONITORING (TOTAL CPU USAGE)
// ==========================================
double GetCPUUsage() {
    static long long last_idle = 0;
    static long long last_total = 0;

    std::ifstream stat_file("/proc/stat");
    if (!stat_file.is_open()) return 0.0;

    std::string cpu_label;
    long long user, nice, system, idle, iowait, irq, softirq, steal;
    
    // Read the first line of /proc/stat which represents total CPU aggregate
    stat_file >> cpu_label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    stat_file.close();

    // Total time is the sum of all fields
    long long current_total = user + nice + system + idle + iowait + irq + softirq + steal;
    long long current_idle = idle + iowait; // iowait is often considered idle time

    double cpu_usage = 0.0;

    if (last_total != 0) {
        long long total_diff = current_total - last_total;
        long long idle_diff = current_idle - last_idle;

        if (total_diff > 0) {
            // Formula: (Total - Idle) / Total
            cpu_usage = 100.0 * (1.0 - static_cast<double>(idle_diff) / total_diff);
        }
    }

    // Update static variables for the next call
    last_total = current_total;
    last_idle = current_idle;

    // Clamp value between 0 and 100
    return std::max(0.0, std::min(100.0, cpu_usage));
}

double GetMemoryUsageMB() {
    std::ifstream file("/proc/self/status");
    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            std::string valueStr = "";
            for (char c : line) if (isdigit(c)) valueStr += c;
            if (!valueStr.empty()) return std::stoll(valueStr) / 1024.0;
        }
    }
    return 0.0;
}
