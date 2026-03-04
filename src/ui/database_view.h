#pragma once
#include "../core/database_manager.h"

// ==========================================
// DATABASE VIEW
// ==========================================
class DatabaseView {
public:
    DatabaseView(DatabaseManager* dbManager);
    
    void Render();
    void RefreshData();
    
private:
    DatabaseManager* db;
    char titleInput[64] = "";
    char descriptionInput[128] = "";
};
