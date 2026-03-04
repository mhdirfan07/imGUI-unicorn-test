#include "widget_gallery_view.h"
#include <cstring>

WidgetGalleryView::WidgetGalleryView() {
    strcpy(strInput, "Hello World");
}

void WidgetGalleryView::Render() {
    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "UI COMPONENT GALLERY");
    ImGui::Separator();

    if (ImGui::BeginTabBar("GalleryTabs")) {
        if (ImGui::BeginTabItem("1. Basic & Text")) {
            ImGui::Spacing();
            ImGui::Text("Regular Text (Default Font 20px)");
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Colored Text");
            ImGui::BulletText("Bullet Point");
            ImGui::Separator();
            if (ImGui::Button("Simple Button")) counter++;
            ImGui::SameLine();
            ImGui::Text("Clicks: %d", counter);
            ImGui::SmallButton("Small Button");
            ImGui::ArrowButton("##left", ImGuiDir_Left);
            ImGui::SameLine();
            ImGui::ArrowButton("##right", ImGuiDir_Right);
            ImGui::Separator();
            ImGui::Checkbox("Checkbox", &check);
            ImGui::RadioButton("Radio A", &radio, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Radio B", &radio, 1);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("2. Numbers & Data")) {
            ImGui::Spacing();
            ImGui::InputText("String Input", strInput, 128);
            ImGui::InputInt("Int Input", &iValue);
            ImGui::SliderInt("Int Slider", &iValue, 0, 1000);
            ImGui::SliderFloat("Float Slider", &fValue, 0.0f, 1.0f);
            ImGui::DragInt("Drag Int", &iValue);
            progress += 0.005f;
            if (progress > 1.0f) progress = 0.0f;
            ImGui::ProgressBar(progress, ImVec2(-1, 0));
            float arr[] = {0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f};
            ImGui::PlotLines("Graph", arr, 7);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("3. Layout & Groups")) {
            ImGui::Spacing();
            if (ImGui::CollapsingHeader("Header 1")) ImGui::Text("Header 1 Content");
            if (ImGui::TreeNode("Folder A")) {
                ImGui::Text("File A.1");
                ImGui::TreePop();
            }
            ImGui::BeginGroup();
            ImGui::Button("A");
            ImGui::Button("B");
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Text("Group");
            ImGui::Text("Right");
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("4. Popups")) {
            ImGui::Spacing();
            ImGui::Button("Hover Me");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Tooltip Appeared!");
            if (ImGui::Button("Open Modal")) ImGui::OpenPopup("Confirmation");
            if (ImGui::BeginPopupModal("Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Are you sure?");
                if (ImGui::Button("OK", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            ImGui::ColorEdit4("Color Picker", col4);
            const char* items[] = {"Item 1", "Item 2"};
            ImGui::Combo("Combo Box", &item_current, items, 2);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}
