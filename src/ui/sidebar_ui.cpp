#include "sidebar_ui.h"

SidebarUI::SidebarUI() : selectedView(0) {}

void SidebarUI::RenderIconButton(const char* icon, const char* label, int viewId, int& selected) {
    ImGui::PushID(viewId);
    
    ImVec2 buttonSize(50, 50);
    ImVec4 activeColor(0.2f, 0.45f, 0.8f, 1.0f);
    ImVec4 inactiveColor(0.3f, 0.3f, 0.35f, 1.0f);
    
    if (selected == viewId) {
        ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.55f, 0.9f, 1.0f));
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, inactiveColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.45f, 1.0f));
    }
    
    if (ImGui::Button(icon, buttonSize)) {
        selected = viewId;
    }
    
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", label);
    }
    
    ImGui::PopStyleColor(2);
    ImGui::PopID();
}

void SidebarUI::Render(int& selectedView) {
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    
    ImGui::BeginChild("Sidebar", ImVec2(70, availSize.y), true, ImGuiWindowFlags_NoScrollbar);
    
    float buttonSize = 50.0f;
    float indent = (70.0f - buttonSize) / 2.0f;
    
    ImGui::Spacing();
    ImGui::Indent(indent);
    
    // Main buttons
    if (ImGui::Button("🏠", ImVec2(buttonSize, buttonSize))) selectedView = 0;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Home");
    
    if (ImGui::Button("📷", ImVec2(buttonSize, buttonSize))) selectedView = 1;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Camera");
    
    if (ImGui::Button("▶", ImVec2(buttonSize, buttonSize))) selectedView = 2;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Play");
    
    if (ImGui::Button("⏺", ImVec2(buttonSize, buttonSize))) selectedView = 3;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Record");
    
    if (ImGui::Button("≡", ImVec2(buttonSize, buttonSize))) selectedView = 4;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Sliders");
    
    if (ImGui::Button("⚙", ImVec2(buttonSize, buttonSize))) selectedView = 5;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Settings");
    
    ImGui::Unindent(indent);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Indent(indent);
    
    // Bottom buttons
    if (ImGui::Button("🚗", ImVec2(buttonSize, buttonSize))) selectedView = 6;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Vehicle");
    
    if (ImGui::Button("🎛", ImVec2(buttonSize, buttonSize))) selectedView = 7;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Control");
    
    ImGui::Unindent(indent);
    ImGui::EndChild();
}
