#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>

// ==========================================
// 2. DATABASE MANAGER
// ==========================================
struct LogRecord {
    int id;
    std::string title;
    std::string description;
    std::string timestamp;
};

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();
    
    void Initialize();
    void Insert(const std::string& title, const std::string& desc);
    void Refresh();
    
    std::vector<LogRecord>& GetData() { return data; }
    
private:
    sqlite3* db;
    std::vector<LogRecord> data;
};
