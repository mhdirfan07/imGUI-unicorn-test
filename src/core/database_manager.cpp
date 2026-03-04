#include "database_manager.h"

DatabaseManager::DatabaseManager() : db(nullptr) {}

DatabaseManager::~DatabaseManager() {
    if (db) sqlite3_close(db);
}

void DatabaseManager::Initialize() {
    sqlite3_open("app_data.db", &db);
    const char* sql = "CREATE TABLE IF NOT EXISTS logs (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, description TEXT, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
    sqlite3_exec(db, sql, 0, 0, 0);
}

void DatabaseManager::Insert(const std::string& title, const std::string& desc) {
    std::string sql = "INSERT INTO logs (title, description) VALUES ('" + title + "', '" + desc + "');";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
}

void DatabaseManager::Refresh() {
    data.clear();
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT * FROM logs ORDER BY id DESC;", -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            LogRecord rec;
            rec.id = sqlite3_column_int(stmt, 0);
            rec.title = std::string((const char*)sqlite3_column_text(stmt, 1));
            rec.description = std::string((const char*)sqlite3_column_text(stmt, 2));
            rec.timestamp = std::string((const char*)sqlite3_column_text(stmt, 3));
            data.push_back(rec);
        }
        sqlite3_finalize(stmt);
    }
}
