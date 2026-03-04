#include "database_view.h"
#include "imgui.h"
#include <cstring>

DatabaseView::DatabaseView(DatabaseManager* dbManager) : db(dbManager) {
    std::memset(titleInput, 0, sizeof(titleInput));
    std::memset(descriptionInput, 0, sizeof(descriptionInput));
}

void DatabaseView::RefreshData() {
    db->Refresh();
}

void DatabaseView::Render() {
    ImGui::BeginChild("Form", ImVec2(0, 150), true);
    ImGui::Text("New Log Input");
    ImGui::InputText("Title", titleInput, 64);
    ImGui::InputText("Description", descriptionInput, 128);
    
    if (ImGui::Button("Save", ImVec2(100, 30))) {
        db->Insert(titleInput, descriptionInput);
        db->Refresh();
        titleInput[0] = 0;
        descriptionInput[0] = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Refresh", ImVec2(100, 30))) {
        db->Refresh();
    }
    ImGui::EndChild();
    
    // Display table
    if (ImGui::BeginTable("Logs", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("Title");
        ImGui::TableSetupColumn("Description");
        ImGui::TableHeadersRow();
        
        for (auto& r : db->GetData()) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", r.id);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", r.timestamp.c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", r.title.c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", r.description.c_str());
        }
        ImGui::EndTable();
    }
}
